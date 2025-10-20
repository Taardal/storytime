#include "st_renderer.h"

#include "system/st_defer.h"
#include "system/st_file_reader.h"

namespace Storytime {

    Renderer::Renderer(const Config& config)
        : config(config),
          swapchain({
              .name = std::format("{} swapchain", config.name),
              .dispatcher = config.dispatcher,
              .window = config.window,
              .device = config.device,
              .surface = config.context->get_surface(),
          }),
          initialization_command_pool({
              .name = std::format("{} initialization command pool", config.name),
              .device = config.device,
              .queue_family_index = config.device->get_graphics_queue_family_index(),
              .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
          }),
          runtime_command_pool({
              .name = std::format("{} runtime command pool", config.name),
              .device = config.device,
              .queue_family_index = config.device->get_graphics_queue_family_index(),
              .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
          }),
          vertex_buffer({
              .name = std::format("{} vertex buffer", config.name),
              .device = config.device,
              .size = sizeof(QuadVertex) * vertices.size(),
          }),
          index_buffer({
              .name = std::format("{} index buffer", config.name),
              .device = config.device,
              .size = sizeof(Index) * indices.size(),
          }),
          uniform_buffers(create_uniform_buffers()),
          frame_descriptor_pool(create_frame_descriptor_pool()),
          frame_descriptor_set_layout(create_frame_descriptor_set_layout()),
          batch_descriptor_pool(create_batch_descriptor_pool()),
          batch_descriptor_set_layout(create_batch_descriptor_set_layout()),
          graphics_pipeline(create_graphics_pipeline()),
          sampler(create_sampler())
    {
        record_and_submit_commands([&](const VulkanCommandBuffer& command_buffer) {
            vertex_buffer.set_vertices(vertices.data(), command_buffer);
            index_buffer.set_indices(indices.data(), command_buffer);
        });

        // Per-frame
        create_sync_objects();
        allocate_frame_command_buffers();
        allocate_frame_descriptor_sets();
        write_frame_descriptors();
        prepare_frames();

        // Per-batch
        batch_vertex_buffers = create_batch_vertex_buffers();
        allocate_batch_descriptor_sets();

        quad_batch.resize(max_quads_per_batch);

        white_texture = std::make_shared<VulkanImage>(VulkanImage({
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
            u32 white_texture_pixels = 0xFFFFFFFF;
            white_texture->set_pixels(on_record_commands_fn, sizeof(white_texture_pixels), &white_texture_pixels);
        });

        texture_batch.resize(max_textures_per_batch);
        for (u32 i = 0; i < texture_batch.size(); i++) {
            texture_batch[i] = white_texture;
        }
    }

    Renderer::~Renderer() {
        ST_ASSERT_VK(config.device->wait_until_idle(), "Device must be able to wait until idle");
        white_texture = nullptr;
        texture_batch.clear();
        quad_batch.clear();
        destroy_sync_objects();
        destroy_sampler();
        destroy_batch_descriptor_set_layout();
        destroy_frame_descriptor_set_layout();
    }

    void Renderer::set_view_projection(const ViewProjection& view_projection) const {
        const Frame& frame = frames.at(current_frame_index);
        const VulkanUniformBuffer& uniform_buffer = *frame.uniform_buffer;
        uniform_buffer.set_uniforms(&view_projection);
    }

    bool Renderer::begin_frame() {
        const VulkanDevice& device = *config.device;
        const Frame& frame = frames.at(current_frame_index);

        if (!swapchain.acquire_frame(frame)) {
            return false;
        }

        frame_start_time = Time::now();

        const VulkanCommandBuffer& command_buffer = frame.command_buffer;
        const VulkanDescriptorSet& descriptor_set = frame.descriptor_set;

        begin_frame_command_buffer(command_buffer);
        device.begin_cmd_label(command_buffer, "Frame commands");

        device.insert_cmd_label(command_buffer, "Begin swapchain render");
        swapchain.begin_render(command_buffer);

        device.insert_cmd_label(command_buffer, "Bind graphics pipeline");
        graphics_pipeline.bind(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS);

        device.insert_cmd_label(command_buffer, "Bind frame descriptor set");
        // descriptor_set.bind(command_buffer, graphics_pipeline.get_pipeline_layout(), VK_PIPELINE_BIND_POINT_GRAPHICS);
        command_buffer.bind_descriptor_sets({
            .pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .pipeline_layout = graphics_pipeline.get_pipeline_layout(),
            .first_set = 0, // Set 0 (per-frame)
            .descriptor_set_count = 1,
            .descriptor_sets = (VkDescriptorSet*) &descriptor_set,
            .dynamic_offset_count = 0,
            .dynamic_offsets = nullptr,
        });

        return true;
    }

    void Renderer::end_frame() {
        const VulkanDevice& device = *config.device;

        const Frame& frame = frames.at(current_frame_index);
        const VulkanCommandBuffer& command_buffer = frame.command_buffer;

        flush_batch();

        device.insert_cmd_label(command_buffer, "End swapchain frame");
        swapchain.end_render(command_buffer);

        device.end_cmd_label(command_buffer);
        end_frame_command_buffer(command_buffer);

        swapchain.present_frame(frame);

        frame_end_time = Time::now();

        previous_frame_index = current_frame_index;
        current_frame_index = (current_frame_index + 1) % config.max_frames_in_flight;

        batch_vertex_buffer_index = 0;
        batch_descriptor_set_index = 0;

        // Metrics

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

    void Renderer::flush_batch() {
        if (quad_batch.empty()) {
            ST_LOG_W("Could not flush empty batch");
            return;
        }

        const VulkanDevice& device = *config.device;

        const Frame& frame = frames.at(current_frame_index);
        const VulkanCommandBuffer& command_buffer = frame.command_buffer;

        const VulkanInstanceBuffer& batch_vertex_buffer = batch_vertex_buffers.at(batch_vertex_buffer_index);
        batch_vertex_buffer.set_vertices(quad_batch.data());

        const VulkanDescriptorSet& batch_descriptor_set = batch_descriptor_sets.at(current_frame_index).at(batch_descriptor_set_index);
        write_batch_descriptors(batch_descriptor_set);

        device.insert_cmd_label(command_buffer, "Bind vertex buffers");
        VkBuffer vertex_buffers[] = {
            vertex_buffer,
            batch_vertex_buffer,
        };
        VkDeviceSize offsets[] = { 0, 0 };
        command_buffer.bind_vertex_buffers({
            .first_binding = 0,
            .binding_count = 2,
            .vertex_buffers = vertex_buffers,
            .offsets = offsets,
        });

        device.insert_cmd_label(command_buffer, "Bind index buffer");
        index_buffer.bind(command_buffer, VK_INDEX_TYPE_UINT16);

        device.insert_cmd_label(command_buffer, "Bind batch descriptor set");
        // batch_descriptor_set.bind(command_buffer, graphics_pipeline.get_pipeline_layout(), VK_PIPELINE_BIND_POINT_GRAPHICS);
        command_buffer.bind_descriptor_sets({
            .pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .pipeline_layout = graphics_pipeline.get_pipeline_layout(),
            .first_set = 1, // Set 1 (per-batch)
            .descriptor_set_count = 1,
            .descriptor_sets = (VkDescriptorSet*) &batch_descriptor_set,
            .dynamic_offset_count = 0,
            .dynamic_offsets = nullptr,
        });

        device.insert_cmd_label(command_buffer, "Draw indexed");
        command_buffer.draw_indexed({
            .index_count = (u32) indices.size(),
            .instance_count = quad_batch_index,
            .first_index = 0,
            .vertex_offset = 0,
            .first_instance = 0,
        });
        config.metrics->draw_calls++;

        batch_vertex_buffer_index++;
        batch_descriptor_set_index++;

        quad_batch_index = 0;
        texture_batch_index = 0;
    }

    void Renderer::render_quad(const Quad& quad) {
        if (quad_batch_index >= quad_batch.size() || texture_batch_index >= texture_batch.size()) {
            flush_batch();
        }

        i32 texture_index = -1;

        // Check if the quad's texture already exists in the texture batch.
        // If the quad has no texture, it will find the index of the first white texture.
        auto texture = quad.texture != nullptr ? quad.texture : white_texture;
        for (i32 i = 0; i < texture_batch.size(); i++) {
            if (texture == texture_batch.at(i)) {
                texture_index = i;
                break;
            }
        }

        // If the texture does not already exist in the texture batch, add it and increment the texture batch index.
        if (texture_index == -1) {
            texture_index = texture_batch_index;
            texture_batch.at(texture_index) = quad.texture;
            texture_batch_index++;
            ST_ASSERT(
                texture_batch_index < max_textures_per_batch,
                "Texture index [" << texture_batch_index << "] must be lower than texture batch size [" << max_textures_per_batch << "]"
            );
            config.metrics->texture_count++;
        }

        // Assign the texture index to the quad instance data.
        // The index will point to a new texture or an existing texture.
        // The existing texture may be a previously submitted texture or the white placeholder texture.
        quad_batch.at(quad_batch_index).texture_index = texture_index;

        quad_batch.at(quad_batch_index).model = quad.model;
        quad_batch.at(quad_batch_index).color = quad.color;
        quad_batch.at(quad_batch_index).texture_rectangle = quad.texture_rectangle;
        quad_batch_index++;

        config.metrics->quad_count++;
        config.metrics->index_count = config.metrics->quad_count * max_indices_per_quad;
        config.metrics->vertex_count = config.metrics->quad_count * max_vertices_per_quad;
    }

    std::vector<VulkanUniformBuffer> Renderer::create_uniform_buffers() {
        u32 uniform_buffer_count = config.max_frames_in_flight;

        std::vector<VulkanUniformBuffer> uniform_buffers;
        uniform_buffers.reserve(uniform_buffer_count);

        for (int i = 0; i < uniform_buffer_count; ++i) {
            uniform_buffers.emplace_back(VulkanUniformBuffer({
                .name = std::format("{} uniform buffer {}/{}", config.name, i + 1, uniform_buffer_count),
                .device = config.device,
                .size = sizeof(ViewProjection),
            }));
        }

        return uniform_buffers;
    }

    VulkanGraphicsPipeline Renderer::create_graphics_pipeline() {
        auto vertex_shader_path = ST_RES_DIR / std::filesystem::path("shaders/quad.vert.spv");
        auto fragment_shader_path = ST_RES_DIR / std::filesystem::path("shaders/quad.frag.spv");

        VkShaderModule vertex_shader = create_shader_module(vertex_shader_path);
        VkShaderModule fragment_shader = create_shader_module(fragment_shader_path);

        std::string vertex_shader_name = std::format("{} vertex shader [{}]", config.name.c_str(), vertex_shader_path.c_str());
        set_object_name(vertex_shader, VK_OBJECT_TYPE_SHADER_MODULE, vertex_shader_name.c_str());

        std::string fragment_shader_name = std::format("{} fragment shader [{}]", config.name.c_str(), fragment_shader_path.c_str());
        set_object_name(fragment_shader, VK_OBJECT_TYPE_SHADER_MODULE, fragment_shader_name.c_str());

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

        ST_DEFER([&] {
            destroy_shader_module(vertex_shader);
            destroy_shader_module(fragment_shader);
        });

        VkVertexInputBindingDescription instance_binding_description{};
        instance_binding_description.binding = 1;
        instance_binding_description.stride = sizeof(InstanceData);
        instance_binding_description.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

        std::vector<VkVertexInputBindingDescription> binding_descriptions = {
            QuadVertex::getBindingDescription(),
            instance_binding_description,
        };

        std::vector<VkVertexInputAttributeDescription> quad_vertex_attribute_descriptions = QuadVertex::getAttributeDescriptions();
        std::vector<VkVertexInputAttributeDescription> attribute_descriptions = {
            quad_vertex_attribute_descriptions[0],
            quad_vertex_attribute_descriptions[1],
            VkVertexInputAttributeDescription{
                .location = 2,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(InstanceData, model) + 0 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 3,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(InstanceData, model) + 1 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 4,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(InstanceData, model) + 2 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 5,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(InstanceData, model) + 3 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 6,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(InstanceData, color) + 0 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 7,
                .binding = 1,
                .format = get_vk_format("vec4"),
                .offset = offsetof(InstanceData, texture_rectangle) + 0 * sizeof(glm::vec4),
            },
            VkVertexInputAttributeDescription{
                .location = 8,
                .binding = 1,
                .format = get_vk_format("float"),
                .offset = offsetof(InstanceData, texture_index) + 0 * sizeof(f32),
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
            .vertex_input_binding_descriptions = binding_descriptions,
            .vertex_input_attribute_descriptions = attribute_descriptions,
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

    VkSampler Renderer::create_sampler() const {
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
        sampler_create_info.maxLod = 0.0f; // VK_LOD_CLAMP_NONE

        VkSampler sampler;

        if (device.create_sampler(sampler_create_info, &sampler) != VK_SUCCESS) {
            ST_THROW("Could not create sampler");
        }

        return sampler;
    }

    void Renderer::destroy_sampler() const {
        const VulkanDevice& device = *config.device;
        device.destroy_sampler(sampler);
    }

    void Renderer::create_sync_objects() {
        const VulkanDevice& device = *config.device;

        u32 sync_object_count = config.max_frames_in_flight;

        image_available_semaphores.resize(sync_object_count);
        render_finished_semaphores.resize(sync_object_count);
        in_flight_fences.resize(sync_object_count);

        VkSemaphoreCreateInfo semaphore_create_info{};
        semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_create_info{};
        fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

        // Create the fence in the signaled state.
        //
        // Rendering a frame starts with waiting for the previous frame to finish rendering using vkWaitForFences(),
        // but when rendering the very first frame, there are no previous frames and vkWaitForFences() will wait and block forever.
        //
        // We solve this by creating the fence in the signaled state, so that the first call to vkWaitForFences() returns immediately
        // since the fence is already signaled.
        //
        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (u32 i = 0; i < sync_object_count; i++) {
            if (device.create_semaphore(semaphore_create_info, &image_available_semaphores[i]) != VK_SUCCESS) {
                ST_THROW("Could not create 'image available' semaphore");
            }

            if (device.create_semaphore(semaphore_create_info, &render_finished_semaphores[i]) != VK_SUCCESS) {
                ST_THROW("Could not create 'render finished' semaphore");
            }

            if (device.create_fence(fence_create_info, &in_flight_fences[i]) != VK_SUCCESS) {
                ST_THROW("Could not create 'in flight' fence");
            }

            std::string image_available_semaphore_name = std::format("{} 'image available' semaphore {}/{}", config.name.c_str(), i + 1, sync_object_count);
            set_object_name(image_available_semaphores[i], VK_OBJECT_TYPE_SEMAPHORE, image_available_semaphore_name.c_str());

            std::string render_finished_semaphore_name = std::format("{} 'render finished' semaphore {}/{}", config.name.c_str(), i + 1, sync_object_count);
            set_object_name(render_finished_semaphores[i], VK_OBJECT_TYPE_SEMAPHORE, render_finished_semaphore_name.c_str());

            std::string in_flight_fence_name = std::format("{} 'in flight' fence {}/{}", config.name.c_str(), i + 1, sync_object_count);
            set_object_name(in_flight_fences[i], VK_OBJECT_TYPE_FENCE, in_flight_fence_name.c_str());
        }
    }

    void Renderer::destroy_sync_objects() const {
        const VulkanDevice& device = *config.device;
        for (u32 i = 0; i < image_available_semaphores.size(); i++) {
            device.destroy_semaphore(image_available_semaphores.at(i));
        }
        for (u32 i = 0; i < render_finished_semaphores.size(); i++) {
            device.destroy_semaphore(render_finished_semaphores.at(i));
        }
        for (u32 i = 0; i < in_flight_fences.size(); i++) {
            device.destroy_fence(in_flight_fences.at(i));
        }
    }

    void Renderer::allocate_frame_command_buffers() {
        const VulkanDevice& device = *config.device;

        u32 command_buffer_count = config.max_frames_in_flight;
        frame_command_buffers.resize(command_buffer_count);

        runtime_command_pool.allocate_command_buffers(frame_command_buffers.size(), frame_command_buffers.data());

        for (u32 i = 0; i < frame_command_buffers.size(); ++i) {
            std::string command_buffer_name = std::format("{} frame command buffer {}/{}", config.name, i + 1, frame_command_buffers.size());
            set_object_name(frame_command_buffers.at(i), VK_OBJECT_TYPE_COMMAND_BUFFER, command_buffer_name.c_str());
        }
    }

    void Renderer::allocate_frame_descriptor_sets() {
        u32 descriptor_set_count = config.max_frames_in_flight;
        frame_descriptor_sets.resize(descriptor_set_count);

        std::vector<VkDescriptorSetLayout> descriptor_set_layouts(descriptor_set_count, frame_descriptor_set_layout);
        frame_descriptor_pool.allocate_descriptor_sets(frame_descriptor_sets.size(), frame_descriptor_sets.data(), descriptor_set_layouts.data());

        for (size_t i = 0; i < frame_descriptor_sets.size(); i++) {
            std::string descriptor_set_name = std::format("{} frame descriptor set {}/{}", config.name.c_str(), i + 1, descriptor_set_count);
            set_object_name(frame_descriptor_sets.at(i), VK_OBJECT_TYPE_DESCRIPTOR_SET, descriptor_set_name.c_str());
        }
    }

    void Renderer::write_frame_descriptors() const {
        const VulkanDevice& device = *config.device;

        for (size_t i = 0; i < frame_descriptor_sets.size(); i++) {
            const VulkanDescriptorSet& descriptor_set = frame_descriptor_sets.at(i);

            VkDescriptorBufferInfo uniform_buffer_descriptor_info{};
            uniform_buffer_descriptor_info.buffer = uniform_buffers.at(i);
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
    }

    void Renderer::prepare_frames() {
        const VulkanDevice& device = *config.device;
        frames.resize(config.max_frames_in_flight);
        for (u32 i = 0; i < frames.size(); ++i) {
            frames[i].graphics_queue = device.get_graphics_queue();
            frames[i].present_queue = device.get_present_queue();
            frames[i].descriptor_set = frame_descriptor_sets[i];
            frames[i].command_buffer = frame_command_buffers[i];
            frames[i].uniform_buffer = &uniform_buffers[i];
            frames[i].in_flight_fence = in_flight_fences[i];
            frames[i].image_available_semaphore = image_available_semaphores[i];
            frames[i].render_finished_semaphore = render_finished_semaphores[i];
        }
    }

    std::vector<VulkanInstanceBuffer> Renderer::create_batch_vertex_buffers() {
        u32 vertex_buffer_count = max_batches_per_frame;

        std::vector<VulkanInstanceBuffer> batch_vertex_buffers;
        batch_vertex_buffers.reserve(vertex_buffer_count);

        for (u32 i = 0; i < vertex_buffer_count; ++i) {
            batch_vertex_buffers.emplace_back(VulkanInstanceBuffer({
                .name = std::format("{} batch vertex buffer {}/{}", config.name, i + 1, vertex_buffer_count),
                .device = config.device,
                .size = sizeof(InstanceData) * max_quads_per_batch,
            }));
        }

        return batch_vertex_buffers;
    }

    void Renderer::allocate_batch_descriptor_sets() {
        batch_descriptor_sets.resize(config.max_frames_in_flight);
        for (u32 frame = 0; frame < config.max_frames_in_flight; frame++) {
            std::vector<VkDescriptorSet>& descriptor_sets = batch_descriptor_sets.at(frame);

            descriptor_sets.resize(max_batches_per_frame);

            std::vector<VkDescriptorSetLayout> descriptor_set_layouts(max_batches_per_frame, batch_descriptor_set_layout);
            batch_descriptor_pool.allocate_descriptor_sets(descriptor_sets.size(), descriptor_sets.data(), descriptor_set_layouts.data());

            for (u32 batch = 0; batch < batch_descriptor_sets.size(); batch++) {
                std::string descriptor_set_name = std::format("{} frame {} batch descriptor set {}/{}", config.name.c_str(), frame + 1, batch + 1, batch_descriptor_sets.size());
                set_object_name(descriptor_sets.at(batch), VK_OBJECT_TYPE_DESCRIPTOR_SET, descriptor_set_name.c_str());
            }
        }
#if 0
        u32 descriptor_set_count = max_batches_per_frame;
        batch_descriptor_sets.resize(descriptor_set_count);

        std::vector<VkDescriptorSetLayout> descriptor_set_layouts(descriptor_set_count, batch_descriptor_set_layout);
        batch_descriptor_pool.allocate_descriptor_sets(batch_descriptor_sets.size(), batch_descriptor_sets.data(), descriptor_set_layouts.data());

        for (size_t i = 0; i < batch_descriptor_sets.size(); i++) {
            std::string descriptor_set_name = std::format("{} batch descriptor set {}/{}", config.name.c_str(), i + 1, batch_descriptor_sets.size());
            set_object_name(batch_descriptor_sets.at(i), VK_OBJECT_TYPE_DESCRIPTOR_SET, descriptor_set_name.c_str());
        }
#endif
    }

    // std::vector<std::vector<VkDescriptorSet>> batch_descriptor_sets(config.max_frames_in_flight);
    // for (u32 i = 0; i < max_batches_per_frame; i++) {
    //     batch_descriptor_sets.resize(max_batches_per_frame);
    //     std::vector<VkDescriptorSetLayout> descriptor_set_layouts(max_batches_per_frame, batch_descriptor_set_layout);
    //     batch_descriptor_pool.allocate_descriptor_sets(batch_descriptor_sets.size(), batch_descriptor_sets.data(), descriptor_set_layouts.data());
    // }

    void Renderer::write_batch_descriptors(const VulkanDescriptorSet& descriptor_set) const {
        const VulkanDevice& device = *config.device;

        std::vector<VkDescriptorImageInfo> image_descriptor_infos(texture_batch.size());
        for (u32 j = 0; j < texture_batch.size(); j++) {
            image_descriptor_infos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            image_descriptor_infos[j].imageView = texture_batch.at(j)->get_view();
            image_descriptor_infos[j].sampler = sampler;
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

    void Renderer::set_object_name(void* object, VkObjectType object_type, const char* object_name) const {
        const VulkanDevice& device = *config.device;
        if (device.set_object_name(object, object_type, object_name) != VK_SUCCESS) {
            ST_THROW("Could not set object name [" << object_name << "]");
        }
    }
}
