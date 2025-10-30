#include "st_renderer.h"

#include "system/st_defer.h"

namespace Storytime {

    Renderer::Renderer(const Config& config)
        : config(config.assert_valid()),
          init_command_pool(create_init_command_pool()),
          frame_command_pool(create_frame_command_pool()),
          frame_descriptor_pool(create_frame_descriptor_pool()),
          frame_descriptor_set_layout(create_frame_descriptor_set_layout()),
          batch_descriptor_pool(create_batch_descriptor_pool()),
          batch_descriptor_set_layout(create_batch_descriptor_set_layout()),
          quad_graphics_pipeline(create_quad_graphics_pipeline()),
          base_quad_vertex_buffer(create_base_quad_vertex_buffer()),
          base_quad_index_buffer(create_base_quad_index_buffer()),
          texture_sampler(create_texture_sampler()),
          placeholder_texture(create_placeholder_texture()),
          frames(create_frames())
    {
        for (Frame& frame : frames) {
            write_frame_descriptors(frame);
        }
    }

    Renderer::~Renderer() {
        destroy_frames();
        destroy_placeholder_texture();
        destroy_texture_sampler();
        destroy_batch_descriptor_set_layout();
        destroy_frame_descriptor_set_layout();
    }

    VkCommandBuffer Renderer::get_frame_command_buffer() const {
        ST_ASSERT_IN_BOUNDS(frame_index, frames);
        const Frame& frame = frames.at(frame_index);
        return frame.command_buffer;
    }

    void Renderer::wait_until_idle() const {
        ST_ASSERT_THROW_VK(config.device->wait_until_idle(), "Device must be able to wait until idle");
    }

    const Frame* Renderer::begin_frame() const {
        const VulkanDevice& device = *config.device;
        VulkanSwapchain& swapchain = *config.swapchain;

        ST_ASSERT_IN_BOUNDS(frame_index, frames);
        const Frame& frame = frames.at(frame_index);

        if (!swapchain.acquire_frame(frame)) {
            return nullptr;
        }

        const VulkanCommandBuffer& command_buffer = frame.command_buffer;

        begin_frame_command_buffer(command_buffer);
        device.begin_cmd_label(command_buffer, "CommandBuffer");

        swapchain.begin_render_pass(command_buffer);

        return &frame;
    }

    void Renderer::end_frame() {
        const VulkanDevice& device = *config.device;
        VulkanSwapchain& swapchain = *config.swapchain;

        ST_ASSERT_IN_BOUNDS(frame_index, frames);
        Frame& frame = frames.at(frame_index);
#if 0
        if (frame.batch_index < frame.batches.size()) {
            Batch& batch = frame.batches.at(frame.batch_index);
            if (batch.quad_index > 0) {
                flush(frame, batch);
            }
        }
#endif
        const VulkanCommandBuffer& command_buffer = frame.command_buffer;

        swapchain.end_render_pass(command_buffer);

        device.end_cmd_label(command_buffer);
        end_frame_command_buffer(command_buffer);

        swapchain.present_frame(frame);

        reset(frame);
        frame_index = (frame_index + 1) % config.frame_count;
    }

    void Renderer::begin_render() const {
        const VulkanDevice& device = *config.device;

        ST_ASSERT_IN_BOUNDS(frame_index, frames);
        const Frame& frame = frames.at(frame_index);

        const VulkanCommandBuffer& command_buffer = frame.command_buffer;

        device.insert_cmd_label(command_buffer, "Bind graphics pipeline");
        command_buffer.bind_pipeline({
            .pipeline = quad_graphics_pipeline,
            .bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS,
        });

        device.insert_cmd_label(command_buffer, "Bind frame descriptor set");
        command_buffer.bind_descriptor_sets({
            .pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .pipeline_layout = quad_graphics_pipeline.get_pipeline_layout(),
            .first_set = 0, // Set 0 (per-frame)
            .descriptor_set_count = 1,
            .descriptor_sets = (VkDescriptorSet*) &frame.descriptor_set,
        });
    }

    void Renderer::end_render() {
        ST_ASSERT_IN_BOUNDS(frame_index, frames);
        Frame& frame = frames.at(frame_index);

        if (frame.batch_index < frame.batches.size()) {
            Batch& batch = frame.batches.at(frame.batch_index);
            if (batch.quad_index > 0) {
                flush(frame, batch);
            }
        }
    }

    void Renderer::set_view_projection(const ViewProjection& view_projection) const {
        ST_ASSERT_IN_BOUNDS(frame_index, frames);
        const Frame& frame = frames.at(frame_index);

#ifdef ST_ENABLE_ASSERT
        for (int i = 0; i < 4; i++) {
            ST_ASSERT(!contains_nan(view_projection.view[i]), "View matrix cannot contain NaN numbers");
            ST_ASSERT(!contains_infinity(view_projection.view[i]), "View matrix cannot contain infinity numbers");
            ST_ASSERT(!contains_nan(view_projection.projection[i]), "Projection matrix cannot contain NaN numbers");
            ST_ASSERT(!contains_infinity(view_projection.projection[i]), "Projection matrix cannot contain infinity numbers");
        }
#endif
        frame.uniform_buffer.set_data(&view_projection);
    }

    void Renderer::render_quad(const Quad& quad) {
        ST_ASSERT_IN_BOUNDS(frame_index, frames);
        Frame& frame = frames.at(frame_index);

        if (frame.batch_index >= frame.batches.size()) {
            ST_LOG_W("Maximum batches exceeded for frame [{}], dropping quad", frame_index);
            return;
        }

        ST_ASSERT_IN_BOUNDS(frame.batch_index, frame.batches);
        Batch& batch = frame.batches.at(frame.batch_index);

        //
        // Determine texture sampler index and add texture to texture batch if needed.
        // 
        // A batch should only contain unique textures (except for the placeholder texture).
        // 
        // The texture index is used for two things:
        // 1. Check if the texture is already added to the batch.
        // 2. Pass it to the fragment shader so it can select the correct texture to sample from. 
        //

        std::vector<Shared<VulkanImage>>& texture_batch = batch.textures;

        const Shared<VulkanImage>& texture = quad.texture != nullptr ? quad.texture : placeholder_texture;
        i32 texture_index = -1;
        for (i32 i = 0; i < texture_batch.size(); i++) {
            if (texture == texture_batch.at(i)) {
                texture_index = i;
                break;
            }
        }
        if (texture_index == -1) {
            texture_index = batch.texture_index;
            ST_ASSERT_IN_BOUNDS(texture_index, texture_batch);
            texture_batch.at(texture_index) = texture;
            batch.texture_index++;
            config.metrics->texture_count++;
        }
        ST_ASSERT(texture_index > -1, "Invalid quad texture index [" << texture_index << "]");

        //
        // Add quad to quad batch as instance data with the texture sampler index.
        //

        std::vector<QuadInstanceData>& quad_batch = batch.quads;

        ST_ASSERT_IN_BOUNDS(batch.quad_index, quad_batch);
        QuadInstanceData& quad_instance_data = quad_batch.at(batch.quad_index);

        quad_instance_data.texture_index = (f32) texture_index;
        quad_instance_data.texture_rectangle = quad.texture_rectangle;
        quad_instance_data.color = quad.color;
        quad_instance_data.model = quad.model;

        batch.quad_index++;
        config.metrics->quad_count++;

        bool batch_is_full = batch.quad_index >= quad_batch.size() || batch.texture_index >= texture_batch.size();
        if (batch_is_full) {
            flush(frame, batch);
        }

        //
        // Metrics
        //

        config.metrics->index_count = config.metrics->quad_count * max_indices_per_quad;
        config.metrics->vertex_count = config.metrics->quad_count * max_vertices_per_quad;
        config.metrics->draw_calls = (config.metrics->quad_count + max_quads_per_batch - 1) / max_quads_per_batch;
    }

    // Flushing a batch means recording the required commands to render it.
    // The commands are submitted and presented when the frame ends.
    void Renderer::flush(Frame& frame, const Batch& batch) const {
        const VulkanDevice& device = *config.device;
        const VulkanCommandBuffer& command_buffer = frame.command_buffer;

        //
        // Vertices & Indices
        //

        // Upload the quad instance data to the instance vertex buffer.
        batch.vertex_buffer.set_data(batch.quads.data());

        // Bind both the base and instance vertex buffers. The base quad will be reused to draw all quad instances.
        device.insert_cmd_label(command_buffer, "Bind vertex buffers");
        VkBuffer vertex_buffers[] = {
            base_quad_vertex_buffer, // Binding 0 (Base vertices)
            batch.vertex_buffer, // Binding 1 (Instance vertices)
        };
        VkDeviceSize vertex_buffer_offsets[] = {
            0, // Binding 0 (Base vertices)
            0 // Binding 1 (Instance vertices)
        };
        command_buffer.bind_vertex_buffers({
            .first_binding = 0, // Binding 0 (Base vertices)
            .binding_count = 2,
            .vertex_buffers = vertex_buffers,
            .offsets = vertex_buffer_offsets,
        });

        // Bind the base quad index buffer that contains the indices for the base quad.
        // We only need indices for the single base quad because all submitted quads are instanced.
        device.insert_cmd_label(command_buffer, "Bind index buffer");
        command_buffer.bind_index_buffer({
            .index_buffer = base_quad_index_buffer,
            .index_type = index_type,
            .offset = 0,
        });

        //
        // Descriptors
        //

        // Write all descriptors that are specific to this batch (f.ex. texture samplers).
        write_batch_descriptors(batch);

        // Bind the descriptor set that contains the descriptors.
        device.insert_cmd_label(command_buffer, "Bind batch descriptor set");
        command_buffer.bind_descriptor_sets({
            .pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .pipeline_layout = quad_graphics_pipeline.get_pipeline_layout(),
            .first_set = 1, // Set 1 (per-batch)
            .descriptor_set_count = 1,
            .descriptor_sets = (VkDescriptorSet*) &batch.descriptor_set,
        });

        //
        // Draw
        //

        // Draw all the instanced quads in the batch using the base quad vertices and indices.
        device.insert_cmd_label(command_buffer, "Draw indexed");
        command_buffer.draw_indexed({
            .index_count = (u32) base_quad_indices.size(),
            .instance_count = batch.quad_index,
            .first_index = 0,
            .vertex_offset = 0,
            .first_instance = 0,
        });

        //
        // End batch
        //

        // Advance to the next batch. Don't need to clean up/reset anything here because no batches are reused in the
        // same frame. A frame will continue to use the next available batch until all batches have been used.
        frame.batch_index++;
    }

    void Renderer::reset(Frame& frame) const {
        // Reset used batches in frame
        for (u32 i = 0; i <= frame.batch_index; i++) {
            Batch& batch = frame.batches.at(i);
            batch.quad_index = 0;
            batch.texture_index = 0;
            for (u32 j = 0; j < batch.textures.size(); j++) {
                batch.textures.at(j) = placeholder_texture;
            }
        }

        // Reset frame
        frame.batch_index = 0;

        // Reset metrics
        config.metrics->draw_calls = 0;
        config.metrics->quad_count = 0;
        config.metrics->index_count = 0;
        config.metrics->vertex_count = 0;
        config.metrics->texture_count = 0;
    }

    void Renderer::begin_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const {
        if (command_buffer.reset() != VK_SUCCESS) {
            ST_THROW("Could not reset frame command buffer");
        }
        if (command_buffer.begin() != VK_SUCCESS) {
            ST_THROW("Could not begin frame command buffer");
        }
    }

    void Renderer::end_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const {
        if (command_buffer.end() != VK_SUCCESS) {
            ST_THROW("Could not end frame command buffer");
        }
    }

    void Renderer::write_frame_descriptors(const Frame& frame) const {
        const VulkanDevice& device = *config.device;

        const VulkanDescriptorSet& descriptor_set = frame.descriptor_set;
        const VulkanUniformBuffer& uniform_buffer = frame.uniform_buffer;

        std::vector<VkDescriptorBufferInfo> buffer_descriptors(1);

        buffer_descriptors[0].buffer = uniform_buffer;
        buffer_descriptors[0].offset = 0;
        buffer_descriptors[0].range = sizeof(ViewProjection);

        VkWriteDescriptorSet descriptor_write{};
        descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write.dstSet = descriptor_set;
        descriptor_write.dstBinding = 0;
        descriptor_write.dstArrayElement = 0;
        descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_write.descriptorCount = buffer_descriptors.size();
        descriptor_write.pBufferInfo = buffer_descriptors.data();

        descriptor_set.write(device, descriptor_write);
    }

    void Renderer::write_batch_descriptors(const Batch& batch) const {
        const VulkanDevice& device = *config.device;

        const VulkanDescriptorSet& descriptor_set = batch.descriptor_set;
        const std::vector<Shared<VulkanImage>>& textures = batch.textures;

        ST_ASSERT(
            textures.size() == max_textures_per_batch,
            "Number of textures [" << textures.size() << "] must be equal to number of sampler descriptors [" << max_textures_per_batch << "]"
        );

        std::vector<VkDescriptorImageInfo> image_descriptor_infos(textures.size());
        for (u32 i = 0; i < image_descriptor_infos.size(); i++) {
            const Shared<VulkanImage>& texture = textures.at(i);
            ST_ASSERT_NOT_NULL(texture);

            VkImageView image_view = texture->get_view();
            ST_ASSERT_NOT_NULL(image_view);

            image_descriptor_infos.at(i).imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            image_descriptor_infos.at(i).imageView = image_view;
            image_descriptor_infos.at(i).sampler = texture_sampler;
        }

        VkWriteDescriptorSet descriptor_write{};
        descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write.dstSet = descriptor_set;
        descriptor_write.dstBinding = 0;
        descriptor_write.dstArrayElement = 0;
        descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptor_write.descriptorCount = image_descriptor_infos.size();
        descriptor_write.pImageInfo = image_descriptor_infos.data();

        descriptor_set.write(device, descriptor_write);
    }

    VulkanCommandPool Renderer::create_init_command_pool() {
        return VulkanCommandPool({
            .name = std::format("{} init command pool", config.name),
            .device = config.device,
            .queue_family_index = config.device->get_graphics_queue_family_index(),
            .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
        });
    }

    VulkanCommandPool Renderer::create_frame_command_pool() {
        return VulkanCommandPool({
            .name = std::format("{} frame command pool", config.name),
            .device = config.device,
            .queue_family_index = config.device->get_graphics_queue_family_index(),
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        });
    }

    VulkanVertexBuffer Renderer::create_base_quad_vertex_buffer() {
        auto vertex_buffer = VulkanVertexBuffer({
            .name = std::format("{} base quad vertex buffer", config.name),
            .device = config.device,
            .size = sizeof(QuadVertex) * base_quad_vertices.size(),
            .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            .memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        });

        init_command_pool.record_and_submit_commands([&](const VulkanCommandBuffer& command_buffer) {
            vertex_buffer.set_data(command_buffer, base_quad_vertices.data());
        });

        return vertex_buffer;
    }

    VulkanIndexBuffer Renderer::create_base_quad_index_buffer() {
        auto index_buffer = VulkanIndexBuffer({
            .name = std::format("{} base quad index buffer", config.name),
            .device = config.device,
            .index_type = index_type,
            .size = sizeof(Index) * base_quad_indices.size(),
            .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            .memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        });

        init_command_pool.record_and_submit_commands([&](const VulkanCommandBuffer& command_buffer) {
            index_buffer.set_data(command_buffer, base_quad_indices.data());
        });

        return index_buffer;
    }

    VulkanGraphicsPipeline Renderer::create_quad_graphics_pipeline() {
        const VulkanDevice& device = *config.device;

        auto vertex_shader_path = ST_RES_DIR / std::filesystem::path("shaders/quad.vert.spv");
        auto fragment_shader_path = ST_RES_DIR / std::filesystem::path("shaders/quad.frag.spv");

        VkShaderModule vertex_shader = create_shader_module(vertex_shader_path);
        VkShaderModule fragment_shader = create_shader_module(fragment_shader_path);

        ST_DEFER([&] {
            destroy_shader_module(vertex_shader);
            destroy_shader_module(fragment_shader);
        });

        std::string vertex_shader_name = std::format("{} quad vertex shader [{}]", config.name.c_str(), vertex_shader_path.c_str());
        if (device.set_object_name(vertex_shader, VK_OBJECT_TYPE_SHADER_MODULE, vertex_shader_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set shader module name [" << vertex_shader_name << "]");
        }

        std::string fragment_shader_name = std::format("{} quad fragment shader [{}]", config.name.c_str(), fragment_shader_path.c_str());
        if (device.set_object_name(fragment_shader, VK_OBJECT_TYPE_SHADER_MODULE, fragment_shader_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set shader module name [" << fragment_shader_name << "]");
        }

        VkPipelineShaderStageCreateInfo vertex_shader_stage_create_info{};
        vertex_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertex_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertex_shader_stage_create_info.module = vertex_shader;
        vertex_shader_stage_create_info.pName = "main";

        VkPipelineShaderStageCreateInfo fragment_shader_stage_create_info{};
        fragment_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragment_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragment_shader_stage_create_info.module = fragment_shader;
        fragment_shader_stage_create_info.pName = "main";

        std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos = {
            vertex_shader_stage_create_info,
            fragment_shader_stage_create_info,
        };

        std::vector<VkVertexInputBindingDescription> vertex_input_binding_descriptions = {
            VkVertexInputBindingDescription{
                .binding = 0,
                .stride = sizeof(QuadVertex),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
            },
            VkVertexInputBindingDescription{
                .binding = 1,
                .stride = sizeof(QuadInstanceData),
                .inputRate = VK_VERTEX_INPUT_RATE_INSTANCE,
            },
        };

        std::vector<VkVertexInputAttributeDescription> vertex_input_attribute_descriptions = {

            // Binding 0: Base quad vertex attributes.
            VkVertexInputAttributeDescription{
                .location = 0,
                .binding = 0,
                .format = get_vk_format("vec4"),
                .offset = offsetof(QuadVertex, position),
            },
            VkVertexInputAttributeDescription{
                .location = 1,
                .binding = 0,
                .format = get_vk_format("vec2"),
                .offset = offsetof(QuadVertex, texture_coordinate),
            },

            // Binding 1: Instance quad vertex attributes.
            VkVertexInputAttributeDescription{
                .location = 2,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(QuadInstanceData, model) + 0 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 3,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(QuadInstanceData, model) + 1 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 4,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(QuadInstanceData, model) + 2 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 5,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(QuadInstanceData, model) + 3 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 6,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(QuadInstanceData, color) + 0 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 7,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(QuadInstanceData, texture_rectangle) + 0 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 8,
                .binding = 1,
                .format = get_vk_format("float"),
                .offset = offsetof(QuadInstanceData, texture_index) + 0 * sizeof(f32),
            },
        };

        std::vector<VkDescriptorSetLayout> descriptor_set_layouts = {
            frame_descriptor_set_layout, // Set 0 (Per-frame)
            batch_descriptor_set_layout, // Set 1 (Per-batch)
        };

        return VulkanGraphicsPipeline({
            .name = std::format("{} quad graphics pipeline", config.name),
            .device = config.device,
            .render_pass = config.swapchain->get_render_pass(),
            .descriptor_set_layouts = descriptor_set_layouts,
            .shader_stage_create_infos = shader_stage_create_infos,
            .vertex_input_binding_descriptions = vertex_input_binding_descriptions,
            .vertex_input_attribute_descriptions = vertex_input_attribute_descriptions,
        });
    }

    VkShaderModule Renderer::create_shader_module(const std::filesystem::path& path) const {
        const VulkanDevice& device = *config.device;
        const FileReader& file_reader = *config.file_reader;

        std::vector<char> shader_bytes = file_reader.read_bytes(path);
        if (shader_bytes.empty()) {
            ST_THROW("Could not read shader [" << path << "]");
        }

        VkShaderModuleCreateInfo shader_module_create_info{};
        shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_module_create_info.codeSize = shader_bytes.size();
        shader_module_create_info.pCode = (const u32*) shader_bytes.data();

        VkShaderModule shader_module;
        if (device.create_shader_module(shader_module_create_info, &shader_module) != VK_SUCCESS) {
            ST_THROW("Could not create shader module");
        }
        return shader_module;
    }

    VulkanDescriptorPool Renderer::create_frame_descriptor_pool() {
        std::vector<VkDescriptorPoolSize> frame_descriptor_pool_sizes(1);

        frame_descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        frame_descriptor_pool_sizes[0].descriptorCount = config.frame_count;

        return VulkanDescriptorPool({
            .name = std::format("{} frame descriptor pool", config.name),
            .device = config.device,
            .max_descriptor_sets = config.frame_count,
            .descriptor_pool_sizes = frame_descriptor_pool_sizes,
        });
    }

    VulkanDescriptorPool Renderer::create_batch_descriptor_pool() {
        std::vector<VkDescriptorPoolSize> batch_descriptor_pool_sizes(1);

        batch_descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        batch_descriptor_pool_sizes[0].descriptorCount = config.frame_count * max_batches_per_frame * max_textures_per_batch;

        return VulkanDescriptorPool({
            .name = std::format("{} batch descriptor pool", config.name),
            .device = config.device,
            .max_descriptor_sets = config.frame_count * max_batches_per_frame,
            .descriptor_pool_sizes = batch_descriptor_pool_sizes,
        });
    }

    VkDescriptorSetLayout Renderer::create_frame_descriptor_set_layout() const {
        const VulkanDevice& device = *config.device;
        const VulkanPhysicalDevice& physical_device = config.device->get_physical_device();
        const VkPhysicalDeviceLimits& physical_device_limits = physical_device.get_properties().limits;

        std::string descriptor_set_layout_name = std::format("{} frame descriptor set layout", config.name);

        u32 uniform_buffer_size = sizeof(ViewProjection);
        u32 max_uniform_buffer_size = physical_device_limits.maxUniformBufferRange;
        if (uniform_buffer_size > max_uniform_buffer_size) {
            ST_THROW("Could not create frame descriptor set layout [" << descriptor_set_layout_name << "] because uniform buffer size [" << uniform_buffer_size << "] exceeded the limit [" << max_uniform_buffer_size << "]");
        }

        std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings(1);

        descriptor_set_layout_bindings[0].binding = 0;
        descriptor_set_layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_set_layout_bindings[0].descriptorCount = 1;
        descriptor_set_layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        descriptor_set_layout_bindings[0].pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info{};
        descriptor_set_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptor_set_layout_create_info.bindingCount = descriptor_set_layout_bindings.size();
        descriptor_set_layout_create_info.pBindings = descriptor_set_layout_bindings.data();

        VkDescriptorSetLayout descriptor_set_layout;

        if (device.create_descriptor_set_layout(descriptor_set_layout_create_info, &descriptor_set_layout) != VK_SUCCESS) {
            ST_THROW("Could not create frame descriptor set layout");
        }

        if (device.set_object_name(descriptor_set_layout, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, descriptor_set_layout_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set frame descriptor set layout name [" << descriptor_set_layout_name << "]");
        }

        return descriptor_set_layout;
    }

    void Renderer::destroy_frame_descriptor_set_layout() const {
        const VulkanDevice& device = *config.device;
        device.destroy_descriptor_set_layout(frame_descriptor_set_layout);
    }

    VkDescriptorSetLayout Renderer::create_batch_descriptor_set_layout() const {
        const VulkanDevice& device = *config.device;
        const VulkanPhysicalDevice& physical_device = config.device->get_physical_device();
        const VkPhysicalDeviceLimits& physical_device_limits = physical_device.get_properties().limits;

        std::string descriptor_set_layout_name = std::format("{} batch descriptor set layout", config.name);

        u32 sampler_count = max_textures_per_batch;
        u32 max_sampler_count = physical_device_limits.maxPerStageDescriptorSampledImages;
        if (sampler_count > max_sampler_count) {
            ST_THROW("Could not create batch descriptor set layout [" << descriptor_set_layout_name << "] because the number of samplers [" << sampler_count << "] exceeded the limit [" << max_sampler_count << "]");
        }

        std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings(1);

        descriptor_set_layout_bindings[0].binding = 0;
        descriptor_set_layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptor_set_layout_bindings[0].descriptorCount = max_textures_per_batch;
        descriptor_set_layout_bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        descriptor_set_layout_bindings[0].pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info{};
        descriptor_set_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptor_set_layout_create_info.bindingCount = descriptor_set_layout_bindings.size();
        descriptor_set_layout_create_info.pBindings = descriptor_set_layout_bindings.data();

        VkDescriptorSetLayout descriptor_set_layout;

        if (device.create_descriptor_set_layout(descriptor_set_layout_create_info, &descriptor_set_layout) != VK_SUCCESS) {
            ST_THROW("Could not create batch descriptor set layout [" << descriptor_set_layout_name << "]");
        }

        if (device.set_object_name(descriptor_set_layout, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, descriptor_set_layout_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set batch descriptor set layout name [" << descriptor_set_layout_name << "]");
        }

        return descriptor_set_layout;
    }

    void Renderer::destroy_batch_descriptor_set_layout() const {
        const VulkanDevice& device = *config.device;
        device.destroy_descriptor_set_layout(batch_descriptor_set_layout);
    }

    void Renderer::destroy_shader_module(VkShaderModule shader_module) const {
        const VulkanDevice& device = *config.device;
        device.destroy_shader_module(shader_module);
    }

    VkSampler Renderer::create_texture_sampler() const {
        const VulkanDevice& device = *config.device;

        VkSamplerCreateInfo sampler_create_info{};
        sampler_create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        sampler_create_info.magFilter = VK_FILTER_NEAREST;
        sampler_create_info.minFilter = VK_FILTER_NEAREST;
        sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        sampler_create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        sampler_create_info.anisotropyEnable = VK_FALSE;
        sampler_create_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        sampler_create_info.unnormalizedCoordinates = VK_FALSE;
        sampler_create_info.compareEnable = VK_FALSE;
        sampler_create_info.compareOp = VK_COMPARE_OP_ALWAYS;
        sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        sampler_create_info.mipLodBias = 0.0f;
        sampler_create_info.minLod = 0.0f; // The lowest mip level that can be sampled (base image).
        sampler_create_info.maxLod = VK_LOD_CLAMP_NONE; // The highest mip level that can be sampled (larger number = smaller mip).

        VkSampler sampler;
        if (device.create_sampler(sampler_create_info, &sampler) != VK_SUCCESS) {
            ST_THROW("Could not create sampler");
        }
        return sampler;
    }

    void Renderer::destroy_texture_sampler() const {
        const VulkanDevice& device = *config.device;
        device.destroy_sampler(texture_sampler);
    }

    Shared<VulkanImage> Renderer::create_placeholder_texture() const {
        auto placeholder_texture = std::make_shared<VulkanImage>(VulkanImage({
            .name = std::format("{} white texture", config.name),
            .device = config.device,
            .width = 1,
            .height = 1,
            .format = VK_FORMAT_R8G8B8A8_SRGB,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .aspect = VK_IMAGE_ASPECT_COLOR_BIT,
            .usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            .mip_levels = 1,
        }));

        init_command_pool.record_and_submit_commands([&](const OnRecordCommandsFn& on_record_commands_fn) {
            u32 white_pixels = 0xFFFFFFFF;
            placeholder_texture->set_pixels(on_record_commands_fn, sizeof(white_pixels), &white_pixels);
        });

        return placeholder_texture;
    }

    void Renderer::destroy_placeholder_texture() {
        placeholder_texture.reset();
    }

    std::vector<Frame> Renderer::create_frames() {
        const VulkanDevice& device = *config.device;

        u32 frame_count = config.frame_count;
        std::vector<Frame> frames(frame_count);

        for (u32 i = 0; i < frames.size(); ++i) {
            Frame& frame = frames.at(i);

            //
            // Queues
            //

            frame.graphics_queue = device.get_graphics_queue();
            frame.present_queue = device.get_present_queue();

            //
            // Buffers & Descriptors
            //

            frame.command_buffer = frame_command_pool.allocate_command_buffer();
            std::string command_buffer_name = std::format("{} frame command buffer {}/{}", config.name.c_str(), i + 1, frame_count);
            if (device.set_object_name(frame.command_buffer, VK_OBJECT_TYPE_COMMAND_BUFFER, command_buffer_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set command buffer name [" << command_buffer_name << "]");
            }

            frame.descriptor_set = frame_descriptor_pool.allocate_descriptor_set(frame_descriptor_set_layout);
            std::string descriptor_set_name = std::format("{} frame descriptor set {}/{}", config.name.c_str(), i + 1, frame_count);
            if (device.set_object_name(frame.descriptor_set, VK_OBJECT_TYPE_DESCRIPTOR_SET, descriptor_set_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set descriptor set name [" << descriptor_set_name << "]");
            }

            frame.uniform_buffer = VulkanUniformBuffer({
                .name = std::format("{} frame uniform buffer {}/{}", config.name, i + 1, frame_count),
                .device = config.device,
                .size = sizeof(ViewProjection),
            });

            //
            // Sync objects
            //

            VkFenceCreateInfo in_flight_fence_create_info{};
            in_flight_fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            in_flight_fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            std::string in_flight_fence_name = std::format("{} frame 'in flight' fence {}/{}", config.name.c_str(), i + 1, frame_count);

            if (device.create_fence(in_flight_fence_create_info, &frame.in_flight_fence) != VK_SUCCESS) {
                ST_THROW("Could not create fence [" << in_flight_fence_name << "]");
            }
            if (device.set_object_name(frame.in_flight_fence, VK_OBJECT_TYPE_FENCE, in_flight_fence_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set fence name [" << in_flight_fence_name << "]");
            }

            VkSemaphoreCreateInfo image_available_semaphore_create_info{};
            image_available_semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            std::string image_available_semaphore_name = std::format("{} frame 'image available' semaphore {}/{}", config.name.c_str(), i + 1, frame_count);

            if (device.create_semaphore(image_available_semaphore_create_info, &frame.image_available_semaphore) != VK_SUCCESS) {
                ST_THROW("Could not create semaphore [" << image_available_semaphore_name << "]");
            }
            if (device.set_object_name(frame.image_available_semaphore, VK_OBJECT_TYPE_SEMAPHORE, image_available_semaphore_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set semaphore name [" << image_available_semaphore_name << "]");
            }

            VkSemaphoreCreateInfo render_finished_semaphore_create_info{};
            render_finished_semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            std::string render_finished_semaphore_name = std::format("{} frame 'render finished' semaphore {}/{}", config.name.c_str(), i + 1, frame_count);

            if (device.create_semaphore(render_finished_semaphore_create_info, &frame.render_finished_semaphore) != VK_SUCCESS) {
                ST_THROW("Could not create semaphore [" << render_finished_semaphore_name << "]");
            }
            if (device.set_object_name(frame.render_finished_semaphore, VK_OBJECT_TYPE_SEMAPHORE, render_finished_semaphore_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set semaphore name [" << render_finished_semaphore_name << "]");
            }

            //
            // Batches
            //

            u32 batch_count = max_batches_per_frame;
            frame.batches.resize(batch_count);

            for (u32 j = 0; j < frame.batches.size(); j++) {
                Batch& batch = frame.batches.at(j);

                batch.vertex_buffer = VulkanVertexBuffer({
                    .name = std::format("{} batch vertex buffer {}/{}", config.name, i + 1, batch_count),
                    .device = config.device,
                    .size = sizeof(QuadInstanceData) * max_quads_per_batch,
                    .memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                });

                batch.descriptor_set = batch_descriptor_pool.allocate_descriptor_set(batch_descriptor_set_layout);

                batch.quads.resize(max_quads_per_batch);

                batch.textures.resize(max_textures_per_batch);
                for (u32 i = 0; i < batch.textures.size(); i++) {
                    batch.textures.at(i) = placeholder_texture;
                }
            }
        }

        return frames;
    }

    void Renderer::destroy_frames() {
        const VulkanDevice& device = *config.device;
        for (Frame& frame : frames) {
            device.destroy_fence(frame.in_flight_fence);
            device.destroy_semaphore(frame.image_available_semaphore);
            device.destroy_semaphore(frame.render_finished_semaphore);
        }
        frames.clear();
    }
}
