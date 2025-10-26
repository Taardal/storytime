#include "st_renderer.h"

#include "system/st_defer.h"
#include "system/st_file_reader.h"

namespace Storytime {

    Renderer::Renderer(const Config& config)
        : config(config),
          swapchain(create_swapchain()),
          initialization_command_pool(create_initialization_command_pool()),
          runtime_command_pool(create_runtime_command_pool()),
          frame_descriptor_pool(create_frame_descriptor_pool()),
          frame_descriptor_set_layout(create_frame_descriptor_set_layout()),
          batch_descriptor_pool(create_batch_descriptor_pool()),
          batch_descriptor_set_layout(create_batch_descriptor_set_layout()),
          quad_graphics_pipeline(create_quad_graphics_pipeline()),
          quad_vertex_buffer(create_quad_vertex_buffer()),
          quad_index_buffer(create_quad_index_buffer()),
          texture_sampler(create_texture_sampler()),
          placeholder_texture(create_placeholder_texture()),
          frames(create_frames())
    {
        for (Frame& frame : frames) {
            write_frame_descriptors(frame.descriptor_set, frame.view_projection_uniform_buffer);
        }
    }

    Renderer::~Renderer() {
        destroy_placeholder_texture();
        destroy_frames();
        destroy_sampler();
        destroy_batch_descriptor_set_layout();
        destroy_frame_descriptor_set_layout();
    }

    void Renderer::wait_until_idle() const {
        ST_ASSERT_THROW_VK(config.device->wait_until_idle(), "Device must be able to wait until idle");
    }

    bool Renderer::begin_frame() {
        ST_ASSERT_IN_BOUNDS(frame_index, frames);
        const Frame& frame = frames.at(frame_index);

        if (!swapchain.acquire_frame(frame)) {
            return false;
        }

        frame_start_time = Time::now();

        const VulkanCommandBuffer& command_buffer = frame.command_buffer;
        const VulkanDescriptorSet& descriptor_set = frame.descriptor_set;
        const VulkanDevice& device = *config.device;

        begin_frame_command_buffer(command_buffer);
        device.begin_cmd_label(command_buffer, "Frame commands");

        device.insert_cmd_label(command_buffer, "Begin swapchain render");
        swapchain.begin_render(command_buffer);

        device.insert_cmd_label(command_buffer, "Bind graphics pipeline");
        quad_graphics_pipeline.bind(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS);

        device.insert_cmd_label(command_buffer, "Bind frame descriptor set");
        // descriptor_set.bind(command_buffer, graphics_pipeline.get_pipeline_layout(), VK_PIPELINE_BIND_POINT_GRAPHICS);
        command_buffer.bind_descriptor_sets({
            .pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .pipeline_layout = quad_graphics_pipeline.get_pipeline_layout(),
            .first_set = 0, // Set 0 (per-frame)
            .descriptor_set_count = 1,
            .descriptor_sets = (VkDescriptorSet*) &descriptor_set,
            .dynamic_offset_count = 0,
            .dynamic_offsets = nullptr,
        });

        return true;
    }

    void Renderer::end_frame() {
        ST_ASSERT_IN_BOUNDS(frame_index, frames);
        Frame& frame = frames.at(frame_index);

        ST_ASSERT_IN_BOUNDS(frame.batch_index, frame.batches);
        Batch& batch = frame.batches.at(frame.batch_index);

        if (batch.quad_index > 0) {
            flush_batch(frame, batch);
        }

        const VulkanCommandBuffer& command_buffer = frame.command_buffer;
        const VulkanDevice& device = *config.device;

        device.insert_cmd_label(command_buffer, "End swapchain frame");
        swapchain.end_render(command_buffer);

        device.end_cmd_label(command_buffer);
        end_frame_command_buffer(command_buffer);

        swapchain.present_frame(frame);

        reset_frame(frame);
        frame_index = (frame_index + 1) % config.max_frames_in_flight;

        frame_end_time = Time::now();

        //
        // Metrics
        //

        config.metrics->frame_duration_ms = Time::as<Microseconds>(frame_end_time - frame_start_time).count() / 1000.0;
        config.metrics->fps = 1.0 / (config.metrics->frame_duration_ms / 1000.0);

        frame_counter++;
        frame_delta_ms += config.metrics->frame_duration_ms;
        if (frame_delta_ms / 1000.0 > 1.0) {
            config.metrics->fpss = frame_counter;
            frame_delta_ms = 0.0;
            frame_counter = 0;
        }

        config.metrics->draw_calls = 0;
        config.metrics->quad_count = 0;
        config.metrics->index_count = 0;
        config.metrics->vertex_count = 0;
        config.metrics->texture_count = 0;
    }

    void Renderer::set_view_projection(const ViewProjection& view_projection) const {
        ST_ASSERT_IN_BOUNDS(frame_index, frames);
        const Frame& frame = frames.at(frame_index);
        frame.view_projection_uniform_buffer.set_uniforms(&view_projection);
    }

    void Renderer::render_quad(const Quad& quad) {
        ST_ASSERT_IN_BOUNDS(frame_index, frames);
        Frame& frame = frames.at(frame_index);

        ST_ASSERT_IN_BOUNDS(frame.batch_index, frame.batches);
        Batch& batch = frame.batches.at(frame.batch_index);

        std::vector<QuadInstanceData>& quad_batch = batch.quads;
        std::vector<std::shared_ptr<VulkanImage>>& texture_batch = batch.textures;

        bool batch_is_full = batch.quad_index == quad_batch.size() - 1 || batch.texture_index == texture_batch.size() - 1;
        if (batch_is_full) {
            flush_batch(frame, batch);
        }

        //
        // Add texture to texture batch and determine texture sampler index.
        //

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
            batch.texture_index++;
            texture_batch.at(texture_index) = texture;
        }
        ST_ASSERT(texture_index > -1, "Invalid quad texture index [" << texture_index << "]");

        //
        // Add quad to quad batch with the texture sampler index.
        //

        ST_ASSERT_IN_BOUNDS(batch.quad_index, quad_batch);
        QuadInstanceData& quad_instance_data = quad_batch.at(batch.quad_index);

        quad_instance_data.texture_index = (f32) texture_index;
        quad_instance_data.texture_rectangle = quad.texture_rectangle;
        quad_instance_data.color = quad.color;
        quad_instance_data.model = quad.model;

        batch.quad_index++;

        //
        // Metrics
        //

        config.metrics->quad_count++;
        config.metrics->index_count = config.metrics->quad_count * max_indices_per_quad;
        config.metrics->vertex_count = config.metrics->quad_count * max_vertices_per_quad;
    }

    void Renderer::flush_batch(Frame& frame, Batch& batch) const {
        if (frame.batch_index >= max_batches_per_frame) {
            ST_THROW("Could not flush batch [" << frame.batch_index + 1 << "] / [" << max_batches_per_frame << "]. All batches has already been flushed for frame [" << frame_index + 1 << "].");
        }

        const VulkanDevice& device = *config.device;
        const VulkanCommandBuffer& command_buffer = frame.command_buffer;

        //
        // Vertices & Indices
        //

        VulkanInstanceBuffer& quad_instance_vertex_buffer = batch.vertex_buffer;
        std::vector<QuadInstanceData>& quad_batch = batch.quads;
        quad_instance_vertex_buffer.set_vertices(quad_batch.data());

        device.insert_cmd_label(command_buffer, "Bind vertex buffers");
        VkBuffer vertex_buffers[] = {
            quad_vertex_buffer,
            quad_instance_vertex_buffer,
        };
        VkDeviceSize offsets[] = { 0, 0 };
        command_buffer.bind_vertex_buffers({
            .first_binding = 0,
            .binding_count = 2,
            .vertex_buffers = vertex_buffers,
            .offsets = offsets,
        });

        device.insert_cmd_label(command_buffer, "Bind index buffer");
        quad_index_buffer.bind(command_buffer, VK_INDEX_TYPE_UINT16);

        //
        // Descriptors
        //

        VulkanDescriptorSet& batch_descriptor_set = batch.descriptor_set;
        std::vector<std::shared_ptr<VulkanImage>>& texture_batch = batch.textures;
        write_batch_descriptors(batch_descriptor_set, texture_batch);

        device.insert_cmd_label(command_buffer, "Bind batch descriptor set");
        // batch_descriptor_set.bind(command_buffer, graphics_pipeline.get_pipeline_layout(), VK_PIPELINE_BIND_POINT_GRAPHICS);
        command_buffer.bind_descriptor_sets({
            .pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .pipeline_layout = quad_graphics_pipeline.get_pipeline_layout(),
            .first_set = 1, // Set 1 (per-batch)
            .descriptor_set_count = 1,
            .descriptor_sets = (VkDescriptorSet*) &batch_descriptor_set,
            .dynamic_offset_count = 0,
            .dynamic_offsets = nullptr,
        });

        //
        // Draw
        //

        device.insert_cmd_label(command_buffer, "Draw indexed");
        command_buffer.draw_indexed({
            .index_count = (u32) indices.size(),
            .instance_count = batch.quad_index,
            .first_index = 0,
            .vertex_offset = 0,
            .first_instance = 0,
        });

        //
        // End batch
        //

        frame.batch_index++;

        //
        // Metrics
        //

        config.metrics->draw_calls++;
    }

    void Renderer::reset_frame(Frame& frame) const {
        frame.batch_index = 0;
        for (u32 i = 0; i < frame.batches.size(); i++) {
            reset_batch(frame.batches.at(i));
        }
    }

    void Renderer::reset_batch(Batch& batch) const {
        batch.quad_index = 0;
        batch.texture_index = 0;
        for (u32 j = 0; j < batch.textures.size(); j++) {
            batch.textures.at(j) = placeholder_texture;
        }
    }

    VulkanSwapchain Renderer::create_swapchain() {
        return VulkanSwapchain({
            .name = std::format("{} swapchain", config.name),
            .dispatcher = config.dispatcher,
            .window = config.window,
            .device = config.device,
            .surface = config.context->get_surface(),
        });
    }

    VulkanCommandPool Renderer::create_initialization_command_pool() {
        return VulkanCommandPool({
            .name = std::format("{} initialization command pool", config.name),
            .device = config.device,
            .queue_family_index = config.device->get_graphics_queue_family_index(),
            .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
        });
    }

    VulkanCommandPool Renderer::create_runtime_command_pool() {
        return VulkanCommandPool({
            .name = std::format("{} runtime command pool", config.name),
            .device = config.device,
            .queue_family_index = config.device->get_graphics_queue_family_index(),
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        });
    }

    VulkanVertexBuffer Renderer::create_quad_vertex_buffer() {
        auto vertex_buffer = VulkanVertexBuffer({
            .name = std::format("{} vertex buffer", config.name),
            .device = config.device,
            .size = sizeof(QuadVertex) * vertices.size(),
        });

        record_and_submit_commands([&](const VulkanCommandBuffer& command_buffer) {
            vertex_buffer.set_vertices(vertices.data(), command_buffer);
        });

        return vertex_buffer;
    }

    VulkanIndexBuffer Renderer::create_quad_index_buffer() {
        auto index_buffer = VulkanIndexBuffer({
            .name = std::format("{} index buffer", config.name),
            .device = config.device,
            .size = sizeof(Index) * indices.size(),
        });

        record_and_submit_commands([&](const VulkanCommandBuffer& command_buffer) {
            index_buffer.set_indices(indices.data(), command_buffer);
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

        std::string vertex_shader_name = std::format("{} vertex shader [{}]", config.name.c_str(), vertex_shader_path.c_str());
        if (device.set_object_name(vertex_shader, VK_OBJECT_TYPE_SHADER_MODULE, vertex_shader_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set shader module name [" << vertex_shader_name << "]");
        }

        std::string fragment_shader_name = std::format("{} fragment shader [{}]", config.name.c_str(), fragment_shader_path.c_str());
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
            //
            // QuadVertex
            //
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
            //
            // QuadInstanceData
            //
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
            frame_descriptor_set_layout, // Set 0
            batch_descriptor_set_layout, // Set 1
        };

        return VulkanGraphicsPipeline({
            .name = std::format("{} graphics pipeline", config.name),
            .device = config.device,
            .render_pass = swapchain.get_render_pass(),
            .descriptor_set_layouts = descriptor_set_layouts,
            .shader_stage_create_infos = shader_stage_create_infos,
            .vertex_input_binding_descriptions = vertex_input_binding_descriptions,
            .vertex_input_attribute_descriptions = vertex_input_attribute_descriptions,
        });
    }

    VkShaderModule Renderer::create_shader_module(const std::filesystem::path& path) const {
        const VulkanDevice& device = *config.device;

        std::vector<char> shader_bytes = FileReader::read_bytes(path);
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
        frame_descriptor_pool_sizes[0].descriptorCount = config.max_frames_in_flight;

        return VulkanDescriptorPool({
            .name = std::format("{} frame descriptor pool", config.name),
            .device = config.device,
            .max_descriptor_sets = config.max_frames_in_flight,
            .descriptor_pool_sizes = frame_descriptor_pool_sizes,
        });
    }

    VulkanDescriptorPool Renderer::create_batch_descriptor_pool() {
        std::vector<VkDescriptorPoolSize> batch_descriptor_pool_sizes(1);

        batch_descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        batch_descriptor_pool_sizes[0].descriptorCount = config.max_frames_in_flight * max_batches_per_frame * max_textures_per_batch;

        return VulkanDescriptorPool({
            .name = std::format("{} batch descriptor pool", config.name),
            .device = config.device,
            .max_descriptor_sets = config.max_frames_in_flight * max_batches_per_frame,
            .descriptor_pool_sizes = batch_descriptor_pool_sizes,
        });
    }

    VkDescriptorSetLayout Renderer::create_frame_descriptor_set_layout() const {
        const VulkanDevice& device = *config.device;

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
        return descriptor_set_layout;
    }

    void Renderer::destroy_frame_descriptor_set_layout() const {
        const VulkanDevice& device = *config.device;
        device.destroy_descriptor_set_layout(frame_descriptor_set_layout);
    }

    VkDescriptorSetLayout Renderer::create_batch_descriptor_set_layout() const {
        const VulkanDevice& device = *config.device;

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
            ST_THROW("Could not create batch descriptor set layout");
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
        sampler_create_info.minLod = 0.0f;
        sampler_create_info.maxLod = VK_LOD_CLAMP_NONE;

        VkSampler sampler;
        if (device.create_sampler(sampler_create_info, &sampler) != VK_SUCCESS) {
            ST_THROW("Could not create sampler");
        }
        return sampler;
    }

    void Renderer::destroy_sampler() const {
        const VulkanDevice& device = *config.device;
        device.destroy_sampler(texture_sampler);
    }

    std::shared_ptr<VulkanImage> Renderer::create_placeholder_texture() const {
        auto placeholder_texture = std::make_shared<VulkanImage>(VulkanImage({
            .name = "WhiteTexture",
            .device = config.device,
            .width = 1,
            .height = 1,
            .format = VK_FORMAT_R8G8B8A8_SRGB,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .aspect = VK_IMAGE_ASPECT_COLOR_BIT,
            .usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            .mip_levels = 1,
        }));

        record_and_submit_commands([&](const OnRecordCommandsFn& on_record_commands_fn) {
            u32 white_pixels = 0xFFFFFFFF;
            placeholder_texture->set_pixels(on_record_commands_fn, sizeof(white_pixels), &white_pixels);
        });

        return placeholder_texture;
    }

    void Renderer::destroy_placeholder_texture() {
        placeholder_texture = nullptr;
    }

    std::vector<Frame> Renderer::create_frames() {
        const VulkanDevice& device = *config.device;

        u32 frame_count = config.max_frames_in_flight;
        std::vector<Frame> frames(frame_count);

        for (u32 i = 0; i < frames.size(); ++i) {
            Frame& frame = frames.at(i);

            frame.graphics_queue = device.get_graphics_queue();
            frame.present_queue = device.get_present_queue();

            frame.command_buffer = runtime_command_pool.allocate_command_buffer();
            std::string command_buffer_name = std::format("{} frame command buffer {}/{}", config.name.c_str(), i + 1, frame_count);
            if (device.set_object_name(frame.command_buffer, VK_OBJECT_TYPE_COMMAND_BUFFER, command_buffer_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set command buffer name [" << command_buffer_name << "]");
            }

            frame.descriptor_set = frame_descriptor_pool.allocate_descriptor_set(frame_descriptor_set_layout);
            std::string descriptor_set_name = std::format("{} frame descriptor set {}/{}", config.name.c_str(), i + 1, frame_count);
            if (device.set_object_name(frame.descriptor_set, VK_OBJECT_TYPE_DESCRIPTOR_SET, descriptor_set_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set descriptor set name [" << descriptor_set_name << "]");
            }

            frame.view_projection_uniform_buffer = VulkanUniformBuffer({
                .name = std::format("{} frame uniform buffer {}/{}", config.name, i + 1, frame_count),
                .device = config.device,
                .size = sizeof(ViewProjection),
            });

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

                batch.vertex_buffer = VulkanInstanceBuffer({
                    .name = std::format("{} batch vertex buffer {}/{}", config.name, i + 1, batch_count),
                    .device = config.device,
                    .size = sizeof(QuadInstanceData) * max_quads_per_batch,
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

    void Renderer::write_frame_descriptors(const VulkanDescriptorSet& descriptor_set, const VulkanUniformBuffer& uniform_buffer) const {
        const VulkanDevice& device = *config.device;

        VkDescriptorBufferInfo uniform_buffer_descriptor_info{};
        uniform_buffer_descriptor_info.buffer = uniform_buffer;
        uniform_buffer_descriptor_info.offset = 0;
        uniform_buffer_descriptor_info.range = sizeof(ViewProjection);

        VkWriteDescriptorSet descriptor_write{};
        descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write.dstSet = descriptor_set;
        descriptor_write.dstBinding = 0;
        descriptor_write.dstArrayElement = 0;
        descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_write.descriptorCount = 1;
        descriptor_write.pBufferInfo = &uniform_buffer_descriptor_info;

        descriptor_set.write(device, descriptor_write);
    }

    void Renderer::write_batch_descriptors(const VulkanDescriptorSet& descriptor_set, const std::vector<std::shared_ptr<VulkanImage>>& textures) const {
        const VulkanDevice& device = *config.device;

        ST_ASSERT(
            textures.size() == max_textures_per_batch,
            "Number of textures [" << textures.size() << "] must be equal to number of descriptors [" << max_textures_per_batch << "]"
        );

        std::vector<VkDescriptorImageInfo> image_descriptor_infos(textures.size());
        for (u32 j = 0; j < image_descriptor_infos.size(); j++) {
            const std::shared_ptr<VulkanImage>& texture = textures.at(j);
            ST_ASSERT_NOT_NULL(texture);

            VkImageView image_view = texture->get_view();
            ST_ASSERT_NOT_NULL(image_view);

            image_descriptor_infos.at(j).imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            image_descriptor_infos.at(j).imageView = image_view;
            image_descriptor_infos.at(j).sampler = texture_sampler;
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

    VulkanCommandBuffer Renderer::begin_one_time_submit_command_buffer() const {
        VulkanCommandBuffer command_buffer = config.command_pool->allocate_command_buffer();

        if (command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) != VK_SUCCESS) {
            ST_THROW("Could not begin one-time-submit command buffer");
        }

        return command_buffer;
    }

    void Renderer::end_one_time_submit_command_buffer(const VulkanCommandBuffer& command_buffer) const {
        const VulkanDevice& device = *config.device;

        if (command_buffer.end() != VK_SUCCESS) {
            ST_THROW("Could not end one-time-submit command buffer");
        }

        VulkanQueue graphics_queue = device.get_graphics_queue();
        if (graphics_queue.submit(command_buffer) != VK_SUCCESS) {
            ST_THROW("Could not submit one-time-submit command buffer to graphics queue");
        }

        if (device.wait_until_queue_idle(graphics_queue) != VK_SUCCESS) {
            ST_THROW("Could not wait for graphics queue to become idle");
        }

        config.command_pool->free_command_buffer(command_buffer);
    }

    void Renderer::record_and_submit_commands(const RecordCommandsFn& record_commands) const {
        VulkanCommandBuffer command_buffer = begin_one_time_submit_command_buffer();
        record_commands(command_buffer);
        end_one_time_submit_command_buffer(command_buffer);
    }

    void Renderer::record_and_submit_commands(const RecordAndSubmitCommandsFn& record_and_submit_commands) const {
        record_and_submit_commands([&](const RecordCommandsFn& record_commands) {
            VulkanCommandBuffer command_buffer = begin_one_time_submit_command_buffer();
            record_commands(command_buffer);
            end_one_time_submit_command_buffer(command_buffer);
        });
    }
}
