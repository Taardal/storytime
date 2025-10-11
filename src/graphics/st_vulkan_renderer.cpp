#include "st_vulkan_renderer.h"

#include "graphics/st_quad_vertex.h"
#include "graphics/st_vulkan_descriptor_set.h"

#include <stb_image.h>

namespace Storytime {

    const std::vector<QuadVertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    VulkanRenderer::VulkanRenderer(const Config& config)
        : config(config),
          context({
              .app_name = config.name,
              .engine_name = std::format("{} engine", config.name),
              .window = config.window,
              .validation_layers_enabled = config.validation_layers_enabled,
          }),
          physical_device({
              .context = &context,
          }),
          device({
              .name = std::format("{} device", config.name),
              .physical_device = &physical_device,
          }),
          swapchain({
              .name = std::format("{} swapchain", config.name),
              .dispatcher = config.dispatcher,
              .window = config.window,
              .device = &device,
              .command_pool = &initialization_command_pool,
              .surface = context.get_surface(),
          }),
          initialization_command_pool({
              .name = std::format("{} initialization command pool", config.name),
              .device = &device,
              .queue_family_index = device.get_graphics_queue_family_index(),
              .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
          }),
          runtime_command_pool({
              .name = std::format("{} render command pool", config.name),
              .device = &device,
              .queue_family_index = device.get_graphics_queue_family_index(),
              .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
          }),
          vertex_buffer({
              .name = std::format("{} vertex buffer", config.name),
              .device = &device,
              .size = sizeof(vertices[0]) * vertices.size(),
          }),
          index_buffer({
              .name = std::format("{} index buffer", config.name),
              .device = &device,
              .size = sizeof(indices[0]) * indices.size(),
          }),
          uniform_buffers(create_uniform_buffers()),
          descriptor_pool(create_descriptor_pool()),
          descriptor_set_layout(create_descriptor_set_layout()),
          graphics_pipeline(create_graphics_pipeline()),
          sampler(create_sampler())
    {
        //
        // Load texture
        //

        auto texture_path = ST_RES_DIR / std::filesystem::path("textures/vulkan_texture.png");

        int32_t width = 0;
        int32_t height = 0;
        int32_t channels = 0;
        int32_t desired_channels = STBI_rgb_alpha;

        stbi_uc* pixel_data = stbi_load(texture_path.c_str(), &width, &height, &channels, desired_channels);
        if (pixel_data == nullptr) {
            ST_THROW("Could not load texture image [" << texture_path << "]");
        }

        // Calculate the size of the image in bytes.
        u64 pixel_data_size = (u64) (width * height * channels);

        // Calculate the number of levels in the mip chain.
        // - The `max` function selects the largest dimension.
        // - The `log2` function calculates how many times that dimension can be divided by 2.
        // - The `floor` function handles cases where the largest dimension is not a power of 2.
        // - 1 is added so that the original image has a mip level.
        u32 mip_levels = (u32) std::floor(std::log2(std::max(width, height))) + 1;

        texture = VulkanImage({
            .name = "FooTexture",
            .device = &device,
            .width = (u32) width,
            .height = (u32) height,
            .format = VK_FORMAT_R8G8B8A8_SRGB,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .aspect = VK_IMAGE_ASPECT_COLOR_BIT,
            .usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            .mip_levels = mip_levels,
        });

        record_and_submit_commands([&](const OnRecordCommandsFn& on_record_commands_fn) {
            texture.set_pixels(on_record_commands_fn, pixel_data_size, pixel_data);
        });

        stbi_image_free(pixel_data);

        //
        // Init
        //

        create_sync_objects();
        allocate_command_buffers();
        allocate_descriptor_sets();
        write_descriptors();
        prepare_frames();

        record_and_submit_commands([&](const VulkanCommandBuffer& command_buffer) {
            vertex_buffer.set_vertices(vertices.data(), command_buffer);
            index_buffer.set_indices(indices.data(), command_buffer);
        });
    }

    VulkanRenderer::~VulkanRenderer() {
        ST_ASSERT_VK(device.wait_until_idle(), "Device must be able to wait until idle");
        destroy_sync_objects();
        destroy_sampler();
        destroy_descriptor_set_layout();
    }

    void VulkanRenderer::begin_frame() {
        const Frame& frame = frames.at(current_frame_index);

        if (!swapchain.acquire_frame(frame)) {
            return;
        }

        begin_frame_command_buffer(frame.command_buffer);
        device.begin_cmd_label(frame.command_buffer, "Frame commands");

        device.insert_cmd_label(frame.command_buffer, "Begin swapchain frame");
        swapchain.begin_render(frame.command_buffer);
    }

    void VulkanRenderer::end_frame() {
        const Frame& frame = frames.at(current_frame_index);

        device.insert_cmd_label(frame.command_buffer, "End swapchain frame");
        swapchain.end_render(frame.command_buffer);

        device.end_cmd_label(frame.command_buffer);
        end_frame_command_buffer(frame.command_buffer);

        swapchain.present_frame(frame);
        current_frame_index = (current_frame_index + 1) % config.max_frames_in_flight;
    }

    void VulkanRenderer::render() const {
        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame_index);

        device.insert_cmd_label(command_buffer, "Bind pipeline");
        graphics_pipeline.bind(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS);

        device.insert_cmd_label(command_buffer, "Bind vertex buffer");
        vertex_buffer.bind(command_buffer);

        device.insert_cmd_label(command_buffer, "Bind index buffer");
        index_buffer.bind(command_buffer, VK_INDEX_TYPE_UINT16);

        device.insert_cmd_label(command_buffer, "Bind descriptor set");
        VulkanDescriptorSet descriptor_set = descriptor_sets.at(current_frame_index);
        descriptor_set.bind(command_buffer, graphics_pipeline.get_pipeline_layout(), VK_PIPELINE_BIND_POINT_GRAPHICS);

        device.insert_cmd_label(command_buffer, "Draw indexed");
        u32 index_count = indices.size();
        u32 instance_count = 1; // Used for instanced rendering. We are not using instanced rendering so we specify 1 instance.
        u32 first_index = 0; // Specifies an offset into the index buffer, using a value of 1 would cause the graphics card to start reading at the second index.
        i32 vertex_offset = 0; // Specifies an offset to add to the indices in the index buffer
        u32 first_instance = 0; // Specifies an offset for instancing, which we're not using.
        command_buffer.draw_indexed(index_count, instance_count, first_index, vertex_offset, first_instance);

        //
        // Update uniform buffer
        //

        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        VkExtent2D swapchain_image_extent = swapchain.get_image_extent();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), swapchain_image_extent.width / (float) swapchain_image_extent.height, 0.1f, 10.0f);

        // GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted compared to Vulkan.
        // The easiest way to compensate for that is to flip the sign on the scaling factor of the Y axis in the projection matrix.
        // If we don't do this, then the image will be rendered upside down.
        ubo.proj[1][1] *= -1;

        const VulkanUniformBuffer& uniform_buffer = uniform_buffers.at(current_frame_index);
        uniform_buffer.set_uniforms(&ubo);
    }

    std::vector<VulkanUniformBuffer> VulkanRenderer::create_uniform_buffers() {
        u32 uniform_buffer_count = config.max_frames_in_flight;

        std::vector<VulkanUniformBuffer> uniform_buffers;
        uniform_buffers.reserve(uniform_buffer_count);

        for (int i = 0; i < uniform_buffer_count; ++i) {
            uniform_buffers.emplace_back(VulkanUniformBuffer({
                .name = std::format("{} uniform buffer {}/{}", config.name, i + 1, uniform_buffer_count),
                .device = &device,
                .size = sizeof(UniformBufferObject),
            }));
        }

        return uniform_buffers;
    }

    VulkanGraphicsPipeline VulkanRenderer::create_graphics_pipeline() {
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

        return VulkanGraphicsPipeline({
            .name = std::format("{} graphics pipeline", config.name),
            .device = &device,
            .render_pass = swapchain.get_render_pass(),
            .descriptor_set_layouts = { descriptor_set_layout },
            .shader_stage_create_infos = shader_stage_create_infos,
            .vertex_input_binding_descriptions = { QuadVertex::getBindingDescription() },
            .vertex_input_attribute_descriptions = QuadVertex::getAttributeDescriptions(),
        });
    }

    VulkanDescriptorPool VulkanRenderer::create_descriptor_pool() {
        std::vector<VkDescriptorPoolSize> descriptor_pool_sizes(2);

        descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_pool_sizes[0].descriptorCount = config.max_frames_in_flight;

        descriptor_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptor_pool_sizes[1].descriptorCount = config.max_frames_in_flight;

        return VulkanDescriptorPool({
            .name = std::format("{} descriptor pool", config.name),
            .device = &device,
            .max_descriptor_sets = config.max_frames_in_flight,
            .descriptor_pool_sizes = descriptor_pool_sizes,
        });
    }

    VkDescriptorSetLayout VulkanRenderer::create_descriptor_set_layout() const {
        std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings(2);

        descriptor_set_layout_bindings[0].binding = 0; // Specifies the binding used in the shader (i.e. `layout(binding = 0)`)
        descriptor_set_layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_set_layout_bindings[0].descriptorCount = 1;
        descriptor_set_layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Specifies in which shader stages the descriptor is going to be referenced.
        descriptor_set_layout_bindings[0].pImmutableSamplers = nullptr;

        descriptor_set_layout_bindings[1].binding = 1;
        descriptor_set_layout_bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptor_set_layout_bindings[1].descriptorCount = 1;
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

    void VulkanRenderer::destroy_descriptor_set_layout() const {
        device.destroy_descriptor_set_layout(descriptor_set_layout);
    }

    VkShaderModule VulkanRenderer::create_shader_module(const std::filesystem::path& path) const {
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

    void VulkanRenderer::destroy_shader_module(VkShaderModule shader_module) const {
        device.destroy_shader_module(shader_module);
    }

    VkSampler VulkanRenderer::create_sampler() const {
        VkSamplerCreateInfo sampler_create_info{};
        sampler_create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        sampler_create_info.magFilter = VK_FILTER_LINEAR;
        sampler_create_info.minFilter = VK_FILTER_LINEAR;
        sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        sampler_create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        sampler_create_info.anisotropyEnable = VK_TRUE;

        // Max quality
        sampler_create_info.maxAnisotropy = physical_device.get_properties().limits.maxSamplerAnisotropy;

        // Specifies which color is returned when sampling beyond the image with clamp to border addressing mode.
        sampler_create_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

        // Specifies which coordinate system you want to use to address texels in an image.
        // If this field is VK_TRUE, then you can simply use coordinates within the [0, texWidth) and [0, texHeight) range.
        // If this field is VK_FALSE, then the texels are addressed using the [0, 1) range on all axes.
        // Real-world applications almost always use normalized coordinates, because then it's possible to use textures of varying resolutions with the exact same coordinates.
        sampler_create_info.unnormalizedCoordinates = VK_FALSE;

        // If a comparison function is enabled, then texels will first be compared to a value, and the result of that comparison is used in filtering operations.
        sampler_create_info.compareEnable = VK_FALSE;
        sampler_create_info.compareOp = VK_COMPARE_OP_ALWAYS;

        sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        sampler_create_info.mipLodBias = 0.0f;
        sampler_create_info.minLod = 0.0f;
        sampler_create_info.maxLod = VK_LOD_CLAMP_NONE;

        VkSampler sampler;

        if (device.create_sampler(sampler_create_info, &sampler) != VK_SUCCESS) {
            ST_THROW("Could not create sampler");
        }

        return sampler;
    }

    void VulkanRenderer::destroy_sampler() const {
        device.destroy_sampler(sampler);
    }

    void VulkanRenderer::create_sync_objects() {
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

    void VulkanRenderer::destroy_sync_objects() const {
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

    void VulkanRenderer::allocate_command_buffers() {
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

    void VulkanRenderer::allocate_descriptor_sets() {
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

    void VulkanRenderer::write_descriptors() const {
        for (size_t i = 0; i < descriptor_sets.size(); i++) {
            const VulkanDescriptorSet& descriptor_set = descriptor_sets.at(i);

            // Specify the buffers that will be bound to the descriptors.

            VkDescriptorBufferInfo uniform_buffer_descriptor_info{};
            uniform_buffer_descriptor_info.buffer = uniform_buffers.at(i);
            uniform_buffer_descriptor_info.offset = 0;
            uniform_buffer_descriptor_info.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo image_descriptor_info{};
            image_descriptor_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            image_descriptor_info.imageView = texture.get_view();
            image_descriptor_info.sampler = sampler;

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
            descriptor_writes[1].descriptorCount = 1;
            descriptor_writes[1].pImageInfo = &image_descriptor_info;

            descriptor_set.write(device, descriptor_writes);
        }
    }

    void VulkanRenderer::prepare_frames() {
        u32 frame_count = config.max_frames_in_flight;
        frames.resize(frame_count);
        for (u32 i = 0; i < frame_count; ++i) {
            frames[i].command_buffer = command_buffers[i];
            frames[i].in_flight_fence = in_flight_fences[i];
            frames[i].image_available_semaphore = image_available_semaphores[i];
            frames[i].render_finished_semaphore = render_finished_semaphores[i];
            frames[i].graphics_queue = device.get_graphics_queue();
            frames[i].present_queue = device.get_present_queue();
        }
    }

    void VulkanRenderer::begin_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const {
        if (command_buffer.reset() != VK_SUCCESS) {
            ST_THROW("Could not reset frame command buffer");
        }
        if (command_buffer.begin() != VK_SUCCESS) {
            ST_THROW("Could not begin frame command buffer");
        }
    }

    void VulkanRenderer::end_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const {
        if (command_buffer.end() != VK_SUCCESS) {
            ST_THROW("Could not end frame command buffer");
        }
    }

    VulkanCommandBuffer VulkanRenderer::begin_one_time_submit_command_buffer() const {
        VulkanCommandBuffer command_buffer = initialization_command_pool.allocate_command_buffer();

        if (command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) != VK_SUCCESS) {
            ST_THROW("Could not begin one-time-submit command buffer");
        }

        return command_buffer;
    }

    void VulkanRenderer::end_one_time_submit_command_buffer(const VulkanCommandBuffer& command_buffer) const {
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

        initialization_command_pool.free_command_buffer(command_buffer);
    }

    void VulkanRenderer::record_and_submit_commands(const RecordCommandsFn& record_commands) const {
        VulkanCommandBuffer command_buffer = begin_one_time_submit_command_buffer();
        record_commands(command_buffer);
        end_one_time_submit_command_buffer(command_buffer);
    }

    void VulkanRenderer::record_and_submit_commands(const RecordAndSubmitCommandsFn& record_and_submit_commands) const {
        record_and_submit_commands([&](const RecordCommandsFn& record_commands) {
            VulkanCommandBuffer command_buffer = begin_one_time_submit_command_buffer();
            record_commands(command_buffer);
            end_one_time_submit_command_buffer(command_buffer);
        });
    }
}
