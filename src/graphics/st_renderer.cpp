#include "st_renderer.h"

#include "graphics/st_view_projection.h"
#include "graphics/st_vulkan_descriptor_set.h"

#include <stb_image.h>

#include "system/st_defer.h"
#include "system/st_file_reader.h"

namespace Storytime {

    Renderer::Renderer(const Config& config)
        : config(config),
          // context({
          //     .app_name = config.name,
          //     .engine_name = std::format("{} engine", config.name),
          //     .window = config.window,
          //     .validation_layers_enabled = config.validation_layers_enabled,
          // }),
          // physical_device({
          //     .context = &context,
          // }),
          // device({
          //     .name = std::format("{} device", config.name),
          //     .physical_device = &physical_device,
          // }),
          swapchain({
              .name = std::format("{} swapchain", config.name),
              .dispatcher = config.dispatcher,
              .window = config.window,
              .device = config.device,
              .command_pool = config.command_pool,
              .surface = config.context->get_surface(),
          }),
          // initialization_command_pool({
          //     .name = std::format("{} initialization command pool", config.name),
          //     .device = &device,
          //     .queue_family_index = device.get_graphics_queue_family_index(),
          //     .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
          // }),
          runtime_command_pool({
              .name = std::format("{} render command pool", config.name),
              .device = config.device,
              .queue_family_index = config.device->get_graphics_queue_family_index(),
              .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
          }),
          vertex_buffer({
              .name = std::format("{} vertex buffer", config.name),
              .device = config.device,
              .size = sizeof(QuadVertex) * vertices.size(),
          }),
          instance_buffer({
              .name = std::format("{} instance buffer", config.name),
              .device = config.device,
              .size = sizeof(InstanceData) * quads_per_batch,
          }),
          index_buffer({
              .name = std::format("{} index buffer", config.name),
              .device = config.device,
              .size = sizeof(Index) * indices.size(),
          }),
          uniform_buffers(create_uniform_buffers()),
          descriptor_pool(create_descriptor_pool()),
          descriptor_set_layout(create_descriptor_set_layout()),
          graphics_pipeline(create_graphics_pipeline()),
          sampler(create_sampler()),
          white_texture(std::make_shared<VulkanImage>(VulkanImage({
              .name = "WhiteTexture",
              .device = config.device,
              .width = 1,
              .height = 1,
              .format = VK_FORMAT_R8G8B8A8_SRGB,
              .tiling = VK_IMAGE_TILING_OPTIMAL,
              .aspect = VK_IMAGE_ASPECT_COLOR_BIT,
              .usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
              .mip_levels = 1,
          })))
    {
        record_and_submit_commands([&](const VulkanCommandBuffer& command_buffer) {
            vertex_buffer.set_vertices(vertices.data(), command_buffer);
            index_buffer.set_indices(indices.data(), command_buffer);
        });

        record_and_submit_commands([&](const OnRecordCommandsFn& on_record_commands_fn) {
            u32 white_texture_pixels = 0xFFFFFFFF;
            white_texture->set_pixels(on_record_commands_fn, sizeof(white_texture_pixels), &white_texture_pixels);
        });

        texture_batch.resize(textures_per_batch);
        for (u32 i = 0; i < textures_per_batch; i++) {
            texture_batch[i] = white_texture;
        }

        quad_batch.resize(quads_per_batch);

        // record_and_submit_commands([&](const VulkanCommandBuffer& command_buffer) {
        //     instance_buffer.set_vertices(quad_batch.data(), command_buffer);
        // });

        //
        // Init
        //

        create_sync_objects();
        allocate_command_buffers();
        allocate_descriptor_sets();
        write_descriptors();
        prepare_frames();
    }

    Renderer::~Renderer() {
        ST_ASSERT_VK(config.device->wait_until_idle(), "Device must be able to wait until idle");
        white_texture = nullptr;
        texture_batch.clear();
        quad_batch.clear();
        destroy_sync_objects();
        destroy_sampler();
        destroy_descriptor_set_layout();
    }

    void Renderer::begin_frame() {
        const VulkanDevice& device = *config.device;

        if (current_frame_index != previous_frame_index) {
            frame_start_time = Time::now();
        }

        const Frame& frame = frames.at(current_frame_index);
        const VulkanCommandBuffer& command_buffer = frame.command_buffer;
        const VulkanDescriptorSet& descriptor_set = frame.descriptor_set;

        if (!swapchain.acquire_frame(frame)) {
            return;
        }

        begin_frame_command_buffer(command_buffer);
        device.begin_cmd_label(command_buffer, "Frame commands");
    }

    void Renderer::end_frame() {
        const VulkanDevice& device = *config.device;

        const Frame& frame = frames.at(current_frame_index);
        const VulkanCommandBuffer& command_buffer = frame.command_buffer;
        const VulkanDescriptorSet& descriptor_set = frame.descriptor_set;
        const VulkanUniformBuffer& uniform_buffer = *frame.uniform_buffer;

        render_batch();

        device.insert_cmd_label(frame.command_buffer, "End swapchain frame");
        swapchain.end_render(frame.command_buffer);

        device.end_cmd_label(frame.command_buffer);
        end_frame_command_buffer(frame.command_buffer);

        swapchain.present_frame(frame);
        frame_end_time = Time::now();

        previous_frame_index = current_frame_index;
        current_frame_index = (current_frame_index + 1) % config.max_frames_in_flight;

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

    void Renderer::render_batch() {
        const VulkanDevice& device = *config.device;

        const Frame& frame = frames.at(current_frame_index);
        const VulkanCommandBuffer& command_buffer = frame.command_buffer;
        const VulkanDescriptorSet& descriptor_set = frame.descriptor_set;
        const VulkanUniformBuffer& uniform_buffer = *frame.uniform_buffer;

        write_descriptorz(); // NOT WHILE DESCRIPTOR SET IS IN USE BY A COMMAND BUFFER
        instance_buffer.set_vertices(quad_batch.data(), command_buffer); // NOT INSIDE AN ACTIVE RENDER PASS

        device.insert_cmd_label(command_buffer, "Begin swapchain frame");
        swapchain.begin_render(command_buffer);

        device.insert_cmd_label(command_buffer, "Bind pipeline");
        graphics_pipeline.bind(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS);

        device.insert_cmd_label(command_buffer, "Bind descriptor set");
        descriptor_set.bind(command_buffer, graphics_pipeline.get_pipeline_layout(), VK_PIPELINE_BIND_POINT_GRAPHICS);

        device.insert_cmd_label(command_buffer, "Bind vertex buffers");
        VkBuffer vertex_buffers[] = {
            vertex_buffer,
            instance_buffer,
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

        device.insert_cmd_label(command_buffer, "Draw indexed");
        command_buffer.draw_indexed({
            .index_count = (u32) indices.size(),
            .instance_count = quad_batch_index,
            .first_index = 0,
            .vertex_offset = 0,
            .first_instance = 0,
        });

        config.metrics->draw_calls++;
        // config.metrics->quad_count += quad_batch_index;
        // config.metrics->index_count += quad_batch_index * indices_per_quad;
        // config.metrics->vertex_count += quad_batch_index * vertices_per_quad;

        quad_batch_index = 0;
        texture_batch_index = 0;
    }

    void Renderer::upload_batch() {
        const Frame& frame = frames.at(current_frame_index);
        const VulkanCommandBuffer& command_buffer = frame.command_buffer;

        instance_buffer.set_vertices(quad_batch.data(), command_buffer);
    }

    void Renderer::upload_full_batch() {
        const Frame& frame = frames.at(current_frame_index);
        const VulkanCommandBuffer& command_buffer = frame.command_buffer;

        swapchain.end_render(command_buffer);
        upload_batch();
        swapchain.begin_render(command_buffer);
    }

#define FOOBAR

    void Renderer::set_view_projection(ViewProjection& view_projection) {
        const Frame& frame = frames.at(current_frame_index);
        const VulkanUniformBuffer& uniform_buffer = *frame.uniform_buffer;
#if 0
        VkExtent2D swapchain_image_extent = swapchain.get_image_extent();
        view_projection.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        view_projection.projection = glm::perspective(glm::radians(45.0f), swapchain_image_extent.width / (float) swapchain_image_extent.height, 0.1f, 10.0f);

        // GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted compared to Vulkan.
        // The easiest way to compensate for that is to flip the sign on the scaling factor of the Y axis in the projection matrix.
        // If we don't do this, then the image will be rendered upside down.
        view_projection.projection[1][1] *= -1;
#endif

#if 0
        glm::mat4 pixel_center_correction = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
        view_projection.projection = pixel_center_correction * view_projection.projection;
#endif

        uniform_buffer.set_uniforms(&view_projection);
    }

    void Renderer::render_quad(const Quad& quad, const glm::vec4& texture_rectangle) {
#ifndef FOO
        // if (quad_batch_index >= quad_batch.size() || texture_batch_index >= texture_batch.size()) {
        //     upload_full_batch();
        // }

        // Shared<Texture> texture = quad.texture != nullptr ? quad.texture : white_texture;
        // f32 texture_index = -1.0f;
        // for (u32 i = 0; i < texture_count; i++) {
        //     if (textures[i] == texture) {
        //         texture_index = (f32) i;
        //         break;
        //     }
        // }
        // if (texture_index == -1.0f) {
        //     texture_index = (f32) texture_count;
        //     textures[texture_count] = texture;
        //     texture_count++;
        // }

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
                texture_batch_index < textures_per_batch,
                "Texture index [" << texture_batch_index << "] must be lower than texture batch size [" << textures_per_batch << "]"
            );
            config.metrics->texture_count++;
        }

        // Assign the texture index to the quad instance data.
        // The index will point to a new texture or an existing texture.
        // The existing texture may be a previously submitted texture or the white placeholde texture.
        quad_batch.at(quad_batch_index).texture_index = texture_index;

        // if (quad.texture != nullptr) {
        //     texture_batch.at(0) = quad.texture;
        //     quad_batch.at(quad_batch_index).texture_index = 0.0f;
        // } else {
        //     texture_batch.at(1) = white_texture;
        //     quad_batch.at(quad_batch_index).texture_index = 1.0f;
        // }

        // bool found_texture_in_batch = false;
        // for (u32 i = 0; i < batch_textures.size(); i++) {
        //     if (quad.texture == batch_textures.at(i)) {
        //         found_texture_in_batch = true;
        //         break;
        //     }
        // }
        // if (!found_texture_in_batch) {
        //     batch_textures.at(texture_batch_index) = quad.texture;
        //     batch_quads.at(batch_index).texture_index = texture_batch_index;
        //     texture_batch_index++;
        //     ST_ASSERT(
        //         texture_batch_index < textures_per_batch,
        //         "Texture index [" << texture_batch_index << "] must be lower than texture batch size [" << textures_per_batch << "]"
        //     );
        // }

        quad_batch.at(quad_batch_index).model = quad.model;
        quad_batch.at(quad_batch_index).color = quad.color;
        quad_batch.at(quad_batch_index).texture_rectangle = texture_rectangle;
        quad_batch_index++;

        // config.metrics->quad_count = quad_batch_index;
        // config.metrics->texture_count = texture_batch_index;
        config.metrics->quad_count++;
        config.metrics->index_count = config.metrics->quad_count * indices_per_quad;
        config.metrics->vertex_count = config.metrics->quad_count * vertices_per_quad;
#endif
    }

    void Renderer::render() const {
#ifdef FOO
        const Frame& frame = frames.at(current_frame_index);
        const VulkanCommandBuffer& command_buffer = frame.command_buffer;
        const VulkanDescriptorSet& descriptor_set = frame.descriptor_set;
        const VulkanUniformBuffer& uniform_buffer = *frame.uniform_buffer;

        device.insert_cmd_label(command_buffer, "Draw indexed");
        command_buffer.draw_indexed({
            .index_count = (u32) indices.size(),
            .instance_count = quad_batch_index,
            .first_index = 0,
            .vertex_offset = 0,
            .first_instance = 0,
        });
#endif
    }

    void Renderer::reset_metrics() {
    #if 0
        config.metrics->draw_calls = 0;
        config.metrics->quad_count = 0;
        config.metrics->index_count = 0;
        config.metrics->vertex_count = 0;
    #endif
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
        const VulkanDevice& device = *config.device;

        auto vertex_shader_path = ST_RES_DIR / std::filesystem::path("shaders/quad.vert.spv");
        auto fragment_shader_path = ST_RES_DIR / std::filesystem::path("shaders/quad.frag.spv");

        VkShaderModule vertex_shader = create_shader_module(vertex_shader_path);
        VkShaderModule fragment_shader = create_shader_module(fragment_shader_path);

        std::string vertex_shader_name = std::format("{} vertex shader [{}]", config.name.c_str(), vertex_shader_path.c_str());
        if (device.set_object_name(vertex_shader, VK_OBJECT_TYPE_SHADER_MODULE, vertex_shader_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set pipeline vertex shader name [" << vertex_shader_name << "]");
        }

        std::string fragment_shader_name = std::format("{} fragment shader [{}]", config.name.c_str(), fragment_shader_path.c_str());
        if (device.set_object_name(fragment_shader, VK_OBJECT_TYPE_SHADER_MODULE, fragment_shader_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set pipeline fragment shader name [" << fragment_shader_name << "]");
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

        return VulkanGraphicsPipeline({
            .name = std::format("{} graphics pipeline", config.name),
            .device = config.device,
            .render_pass = swapchain.get_render_pass(),
            .descriptor_set_layouts = { descriptor_set_layout },
            .shader_stage_create_infos = shader_stage_create_infos,
            .vertex_input_binding_descriptions = binding_descriptions,
            .vertex_input_attribute_descriptions = attribute_descriptions,
        });
    }

    VulkanDescriptorPool Renderer::create_descriptor_pool() {
        std::vector<VkDescriptorPoolSize> descriptor_pool_sizes(2);

        descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_pool_sizes[0].descriptorCount = config.max_frames_in_flight;

        descriptor_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptor_pool_sizes[1].descriptorCount = config.max_frames_in_flight * textures_per_batch;

        return VulkanDescriptorPool({
            .name = std::format("{} descriptor pool", config.name),
            .device = config.device,
            .max_descriptor_sets = config.max_frames_in_flight,
            .descriptor_pool_sizes = descriptor_pool_sizes,
        });
    }

    VkDescriptorSetLayout Renderer::create_descriptor_set_layout() const {
        const VulkanDevice& device = *config.device;

        std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings(2);

        descriptor_set_layout_bindings[0].binding = 0; // Specifies the binding used in the shader (i.e. `layout(binding = 0)`)
        descriptor_set_layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_set_layout_bindings[0].descriptorCount = 1;
        descriptor_set_layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Specifies in which shader stages the descriptor is going to be referenced.
        descriptor_set_layout_bindings[0].pImmutableSamplers = nullptr;

        descriptor_set_layout_bindings[1].binding = 1;
        descriptor_set_layout_bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptor_set_layout_bindings[1].descriptorCount = textures_per_batch;
        descriptor_set_layout_bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        descriptor_set_layout_bindings[1].pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info{};
        descriptor_set_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptor_set_layout_create_info.bindingCount = descriptor_set_layout_bindings.size();
        descriptor_set_layout_create_info.pBindings = descriptor_set_layout_bindings.data();

        VkDescriptorSetLayout descriptor_set_layout;
        if (device.create_descriptor_set_layout(descriptor_set_layout_create_info, &descriptor_set_layout) != VK_SUCCESS) {
            ST_THROW("Could not create graphics pipeline descriptor set layout");
        }

        return descriptor_set_layout;
    }

    void Renderer::destroy_descriptor_set_layout() const {
        const VulkanDevice& device = *config.device;
        device.destroy_descriptor_set_layout(descriptor_set_layout);
    }

    VkShaderModule Renderer::create_shader_module(const std::filesystem::path& path) const {
        const VulkanDevice& device = *config.device;

        std::vector<char> shader_bytes = FileReader::read_bytes(path);
        if (shader_bytes.empty()) {
            ST_THROW("Could not read graphics pipeline shader [" << path << "]");
        }

        VkShaderModuleCreateInfo shader_module_create_info{};
        shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_module_create_info.codeSize = shader_bytes.size();
        shader_module_create_info.pCode = (const u32*) shader_bytes.data();

        VkShaderModule shader_module;
        if (device.create_shader_module(shader_module_create_info, &shader_module) != VK_SUCCESS) {
            ST_THROW("Could not create graphics shader module");
        }
        return shader_module;
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
            if (device.set_object_name(image_available_semaphores[i], VK_OBJECT_TYPE_SEMAPHORE, image_available_semaphore_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set 'image available' semaphore name [" << image_available_semaphore_name << "]");
            }

            std::string render_finished_semaphore_name = std::format("{} 'render finished' semaphore {}/{}", config.name.c_str(), i + 1, sync_object_count);
            if (device.set_object_name(render_finished_semaphores[i], VK_OBJECT_TYPE_SEMAPHORE, render_finished_semaphore_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set 'render finished' semaphore name [" << render_finished_semaphore_name << "]");
            }

            std::string in_flight_fence_name = std::format("{} 'in flight' fence {}/{}", config.name.c_str(), i + 1, sync_object_count);
            if (device.set_object_name(in_flight_fences[i], VK_OBJECT_TYPE_FENCE, in_flight_fence_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set 'in flight' fence name [" << in_flight_fence_name << "]");
            }
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

    void Renderer::allocate_command_buffers() {
        const VulkanDevice& device = *config.device;

        u32 command_buffer_count = config.max_frames_in_flight;
        command_buffers.resize(command_buffer_count);

        runtime_command_pool.allocate_command_buffers(command_buffers.size(), command_buffers.data());

        for (u32 i = 0; i < command_buffer_count; ++i) {
            std::string command_buffer_name = std::format("{} command buffer {}/{}", config.name, i + 1, command_buffer_count);
            if (device.set_object_name(command_buffers.at(i), VK_OBJECT_TYPE_COMMAND_BUFFER, command_buffer_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set command buffer name [" << command_buffer_name << "]");
            }
        }
    }

    void Renderer::allocate_descriptor_sets() {
        const VulkanDevice& device = *config.device;

        u32 descriptor_set_count = config.max_frames_in_flight;
        descriptor_sets.resize(descriptor_set_count);

        std::vector<VkDescriptorSetLayout> descriptor_set_layouts(descriptor_set_count, descriptor_set_layout);
        descriptor_pool.allocate_descriptor_sets(descriptor_sets.size(), descriptor_sets.data(), descriptor_set_layouts.data());

        for (size_t i = 0; i < descriptor_sets.size(); i++) {
            std::string descriptor_set_name = std::format("{} descriptor set {}/{}", config.name.c_str(), i + 1, descriptor_set_count);
            if (device.set_object_name(descriptor_sets.at(i), VK_OBJECT_TYPE_DESCRIPTOR_SET, descriptor_set_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set 'image available' semaphore name [" << descriptor_set_name << "]");
            }
        }
    }

    void Renderer::write_descriptors() const {
        const VulkanDevice& device = *config.device;

        for (size_t i = 0; i < descriptor_sets.size(); i++) {
            const VulkanDescriptorSet& descriptor_set = descriptor_sets.at(i);

            // Specify the buffers that will be bound to the descriptors.

            VkDescriptorBufferInfo uniform_buffer_descriptor_info{};
            uniform_buffer_descriptor_info.buffer = uniform_buffers.at(i);
            uniform_buffer_descriptor_info.offset = 0;
            uniform_buffer_descriptor_info.range = sizeof(ViewProjection);

            std::vector<VkDescriptorImageInfo> image_descriptor_infos(texture_batch.size());
            for (u32 j = 0; j < texture_batch.size(); j++) {
                image_descriptor_infos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                image_descriptor_infos[j].imageView = texture_batch.at(j)->get_view();
                image_descriptor_infos[j].sampler = sampler;
            }

            // Bind the buffers to the descriptors by writing to the desriptor set

            std::vector<VkWriteDescriptorSet> descriptor_writes(2);

            descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptor_writes[0].dstSet = descriptor_set; // The descriptor set being updated
            descriptor_writes[0].dstBinding = 0; // Specifies the binding used in the shader (i.e. `layout(binding = 0)`).
            descriptor_writes[0].dstArrayElement = 0; // This descriptor is not an array, so just use start index.
            descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // Must match descriptor set layout
            descriptor_writes[0].descriptorCount = 1; // How many descriptors to write to.
            descriptor_writes[0].pBufferInfo = &uniform_buffer_descriptor_info; // The buffer the descriptor will use.

            descriptor_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptor_writes[1].dstSet = descriptor_set;
            descriptor_writes[1].dstBinding = 1;
            descriptor_writes[1].dstArrayElement = 0;
            descriptor_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptor_writes[1].descriptorCount = image_descriptor_infos.size();
            descriptor_writes[1].pImageInfo = image_descriptor_infos.data();

            descriptor_set.write(device, descriptor_writes);
        }
    }

    void Renderer::write_descriptorz() const {
        const VulkanDevice& device = *config.device;

        const Frame& frame = frames.at(current_frame_index);
        const VulkanUniformBuffer& uniform_buffer = *frame.uniform_buffer;
        const VulkanDescriptorSet& descriptor_set = frame.descriptor_set;

        VkDescriptorBufferInfo uniform_buffer_descriptor_info{};
        uniform_buffer_descriptor_info.buffer = uniform_buffer;
        uniform_buffer_descriptor_info.offset = 0;
        uniform_buffer_descriptor_info.range = sizeof(ViewProjection);

        std::vector<VkDescriptorImageInfo> image_descriptor_infos(texture_batch.size());
        for (u32 j = 0; j < texture_batch.size(); j++) {
            image_descriptor_infos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            image_descriptor_infos[j].imageView = texture_batch.at(j)->get_view();
            image_descriptor_infos[j].sampler = sampler;
        }

        // Bind the buffers to the descriptors by writing to the desriptor set

        std::vector<VkWriteDescriptorSet> descriptor_writes(2);

        descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_writes[0].dstSet = descriptor_set; // The descriptor set being updated
        descriptor_writes[0].dstBinding = 0; // Specifies the binding used in the shader (i.e. `layout(binding = 0)`).
        descriptor_writes[0].dstArrayElement = 0; // This descriptor is not an array, so just use start index.
        descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // Must match descriptor set layout
        descriptor_writes[0].descriptorCount = 1; // How many descriptors to write to.
        descriptor_writes[0].pBufferInfo = &uniform_buffer_descriptor_info; // The buffer the descriptor will use.

        descriptor_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_writes[1].dstSet = descriptor_set;
        descriptor_writes[1].dstBinding = 1;
        descriptor_writes[1].dstArrayElement = 0;
        descriptor_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptor_writes[1].descriptorCount = image_descriptor_infos.size();
        descriptor_writes[1].pImageInfo = image_descriptor_infos.data();

        descriptor_set.write(device, descriptor_writes);
    }

    void Renderer::prepare_frames() {
        const VulkanDevice& device = *config.device;
        frames.resize(config.max_frames_in_flight);
        for (u32 i = 0; i < frames.size(); ++i) {
            frames[i].graphics_queue = device.get_graphics_queue();
            frames[i].present_queue = device.get_present_queue();
            frames[i].descriptor_set = descriptor_sets[i];
            frames[i].command_buffer = command_buffers[i];
            frames[i].uniform_buffer = &uniform_buffers[i];
            frames[i].in_flight_fence = in_flight_fences[i];
            frames[i].image_available_semaphore = image_available_semaphores[i];
            frames[i].render_finished_semaphore = render_finished_semaphores[i];
        }
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
