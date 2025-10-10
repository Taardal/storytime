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
              .window = config.window,
              .app_name = config.name,
              .engine_name = std::format("{} engine", config.name),
              .validation_layers_enabled = config.validation_layers_enabled,
          }),
          physical_device({
              .context = &context,
          }),
          device({
              .physical_device = &physical_device,
              .name = std::format("{} device", config.name),
          }),
          swapchain({
              .dispatcher = config.dispatcher,
              .window = config.window,
              .context = &context,
              .physical_device = &physical_device,
              .device = &device,
              .command_pool = &initialization_command_pool,
              .name = std::format("{} swapchain", config.name),
              .max_frames_in_flight = config.max_frames_in_flight,
          }),
          initialization_command_pool({
              .device = &device,
              .name = std::format("{} initialization command pool", config.name),
              .queue_family_index = device.get_graphics_queue_family_index(),
              .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
          }),
          runtime_command_pool({
              .device = &device,
              .name = std::format("{} render command pool", config.name),
              .queue_family_index = device.get_graphics_queue_family_index(),
              .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
          }),
          vertex_buffer({
              .device = &device,
              .name = std::format("{} vertex buffer", config.name),
              .size = sizeof(vertices[0]) * vertices.size(),
          }),
          index_buffer({
              .device = &device,
              .name = std::format("{} index buffer", config.name),
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
            .device = &device,
            .name = "FooTexture",
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

        allocate_command_buffers();
        allocate_descriptor_sets();
        write_descriptors();

        record_and_submit_commands([&](const VulkanCommandBuffer& command_buffer) {
            vertex_buffer.set_vertices(vertices.data(), command_buffer);
            index_buffer.set_indices(indices.data(), command_buffer);
        });
    }

    VulkanRenderer::~VulkanRenderer() {
        ST_ASSERT_VK(device.wait_until_idle(), "Device must be able to wait until idle");
        destroy_sampler();
        destroy_descriptor_set_layout();
    }

    void VulkanRenderer::begin_frame() {
        if (!swapchain.acquire_frame(current_frame_index)) {
            return;
        }

        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame_index);
        begin_frame_command_buffer(command_buffer);
        device.begin_cmd_label(command_buffer, "CommandBuffer");

        device.insert_cmd_label(command_buffer, "Begin swapchain frame");
        swapchain.begin_frame(command_buffer);
    }

    void VulkanRenderer::end_frame() {
        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame_index);

        device.insert_cmd_label(command_buffer, "End swapchain frame");
        swapchain.end_frame(command_buffer);

        device.end_cmd_label(command_buffer);
        end_frame_command_buffer(command_buffer);

        swapchain.present_frame(current_frame_index, command_buffer);
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
                .device = &device,
                .name = std::format("{} uniform buffer {}/{}", config.name, i + 1, uniform_buffer_count),
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
            .device = &device,
            .name = std::format("{} graphics pipeline", config.name),
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
            .device = &device,
            .name = std::format("{} descriptor pool", config.name),
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

    void VulkanRenderer::begin_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const {
        if (command_buffer.reset() != VK_SUCCESS) {
            ST_THROW("Could not reset command buffer");
        }
        if (command_buffer.begin() != VK_SUCCESS) {
            ST_THROW("Could not begin command buffer");
        }
    }

    void VulkanRenderer::end_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const {
        if (command_buffer.end() != VK_SUCCESS) {
            ST_THROW("Could not end command buffer");
        }
    }

    VulkanCommandBuffer VulkanRenderer::begin_one_time_submit_command_buffer() const {
        VulkanCommandBuffer command_buffer = initialization_command_pool.allocate_command_buffer();

        if (command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) != VK_SUCCESS) {
            ST_THROW("Could not begin command buffer");
        }

        return command_buffer;
    }

    void VulkanRenderer::end_one_time_submit_command_buffer(const VulkanCommandBuffer& command_buffer) const {
        if (command_buffer.end() != VK_SUCCESS) {
            ST_THROW("Could not end command buffer");
        }

        VulkanQueue graphics_queue = device.get_graphics_queue();
        if (graphics_queue.submit(command_buffer) != VK_SUCCESS) {
            ST_THROW("Could not submit command buffer to graphics queue");
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
