#include "st_vulkan_renderer.h"

#include "st_quad_vertex.h"

namespace Storytime {

    const std::vector<QuadVertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<u16> indices = {
        0, 1, 2, 2, 3, 0
    };

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    VulkanRenderer::VulkanRenderer(const Config& config)
        : config(config),
          instance({
              .window = config.window,
              .app_name = config.name,
              .engine_name = std::format("{} engine", config.name),
              .validation_layers_enabled = config.validation_layers_enabled,
          }),
          physical_device({
              .instance = &instance,
          }),
          device({
              .physical_device = &physical_device,
              .name = std::format("{} device", config.name),
          }),
          swapchain({
              .dispatcher = config.dispatcher,
              .window = config.window,
              .physical_device = &physical_device,
              .device = &device,
              .surface = instance.get_surface(),
              .name = std::format("{} swapchain", config.name),
              .max_frames_in_flight = config.max_frames_in_flight,
          }),
          graphics_pipeline({
              .device = &device,
              .swapchain = &swapchain,
              .name = std::format("{} graphics pipeline", config.name),
              .vertex_shader_path = ST_RES_DIR / std::filesystem::path("shaders/simple.vert.spv"),
              .fragment_shader_path = ST_RES_DIR / std::filesystem::path("shaders/simple.frag.spv"),
              .vertex_input_binding_description = QuadVertex::getBindingDescription(),
              .vertex_input_attribute_descriptions = QuadVertex::getAttributeDescriptions(),
              .descriptor_set_layout_bindings = get_descriptor_set_layout_bindings(),
          }),
          render_command_pool({
              .device = &device,
              .name = std::format("{} render command pool", config.name),
              .queue_family_index = physical_device.get_queue_family_indices().graphics_family.value(),
              .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
          }),
          transient_command_pool({
              .device = &device,
              .name = std::format("{} transient command pool", config.name),
              .queue_family_index = physical_device.get_queue_family_indices().graphics_family.value(),
              .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
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
          })
    {
        allocate_command_buffers();
        initialize_queues();
        create_uniform_buffers();

        do_commands([&](const VulkanCommandBuffer& command_buffer) {
            vertex_buffer.set_vertices(vertices.data(), command_buffer);
            index_buffer.set_indices(indices.data(), command_buffer);
        });

        for (size_t i = 0; i < config.max_frames_in_flight; i++) {
            VkDescriptorBufferInfo descriptor_buffer_info{};
            descriptor_buffer_info.buffer = uniform_buffers.at(i);
            descriptor_buffer_info.offset = 0;
            descriptor_buffer_info.range = sizeof(UniformBufferObject);

            VkWriteDescriptorSet descriptor_write{};
            descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptor_write.dstSet = descriptor_sets[i];
            descriptor_write.dstBinding = 0;
            descriptor_write.dstArrayElement = 0;
            descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptor_write.descriptorCount = 1;
            descriptor_write.pBufferInfo = &descriptor_buffer_info;
            descriptor_write.pImageInfo = nullptr;
            descriptor_write.pTexelBufferView = nullptr;

            u32 descriptor_write_count = 1;
            u32 descriptor_copy_count = 0;
            VkCopyDescriptorSet* descriptor_copies = nullptr;
            device.update_descriptor_sets(descriptor_write_count, &descriptor_write, descriptor_copy_count, descriptor_copies);
        }
    }

    VulkanRenderer::~VulkanRenderer() {
        ST_ASSERT_VK(device.wait_until_idle(), "Device must be able to wait until idle");
    }

    void VulkanRenderer::begin_frame() {
        if (!swapchain.acquire_frame(current_frame_index)) {
            return;
        }

        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame_index);
        begin_command_buffer(command_buffer);

        swapchain.begin_frame(command_buffer);
    }

    void VulkanRenderer::end_frame() {
        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame_index);

        swapchain.end_frame(command_buffer);
        end_command_buffer(command_buffer);

        swapchain.present_frame(current_frame_index, command_buffer);
        current_frame_index = (current_frame_index + 1) % config.max_frames_in_flight;
    }

    void VulkanRenderer::render() {
        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame_index);

        device.insert_cmd_label(command_buffer, "Bind pipeline");
        graphics_pipeline.bind(command_buffer);

        device.insert_cmd_label(command_buffer, "Bind vertex buffer");
        vertex_buffer.bind(command_buffer);

        device.insert_cmd_label(command_buffer, "Bind index buffer");
        index_buffer.bind(command_buffer, VK_INDEX_TYPE_UINT16);

        //
        // Bind descriptor sets
        //

        VkPipelineBindPoint pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS;
        VkPipelineLayout pipeline_layout = graphics_pipeline.get_layout();
        u32 first_set = 0;
        u32 descriptor_set_count = 1;
        const VkDescriptorSet* descriptor_sets = &descriptor_sets.at(current_frame_index);
        u32 dynamic_offset_count = 0;
        const u32* dynamic_offsets = nullptr;
        command_buffer.bind_descriptor_sets(
            pipeline_bind_point,
            pipeline_layout,
            first_set,
            descriptor_set_count,
            descriptor_sets,
            dynamic_offset_count,
            dynamic_offsets
        );

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

        // GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted. The easiest way to
        // compensate for that is to flip the sign on the scaling factor of the Y axis in the projection matrix. If we don't do this,
        // then the image will be rendered upside down.
        ubo.proj[1][1] *= -1;

        uniform_buffers.at(current_frame_index).set_uniforms(&ubo);
    }

    void VulkanRenderer::allocate_command_buffers() {
        command_buffers.resize(config.max_frames_in_flight);
        render_command_pool.allocate_command_buffers(command_buffers.size(), command_buffers.data());

        for (u32 i = 0; i < command_buffers.size(); ++i) {
            std::string command_buffer_name = std::format("{} command buffer {}/{}", config.name, i + 1, command_buffers.size());
            if (device.set_object_name(command_buffers.at(i), VK_OBJECT_TYPE_COMMAND_BUFFER, command_buffer_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set command buffer name [" << command_buffer_name << "]");
            }
        }
    }

    void VulkanRenderer::initialize_queues() {
        const QueueFamilyIndices& queue_family_indices = physical_device.get_queue_family_indices();
        graphics_queue = device.get_queue(queue_family_indices.graphics_family.value());
        present_queue = device.get_queue(queue_family_indices.present_family.value());

        std::string graphics_queue_name = std::format("{} graphics queue", config.name);
        if (device.set_object_name(graphics_queue, VK_OBJECT_TYPE_QUEUE, config.name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set Vulkan device graphics queue name [" << graphics_queue_name << "]");
        }

        std::string present_queue_name = std::format("{} present queue", config.name);
        if (device.set_object_name(present_queue, VK_OBJECT_TYPE_QUEUE, config.name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set Vulkan device present queue name [" << present_queue_name << "]");
        }
    }

    void VulkanRenderer::create_uniform_buffers() {
        uniform_buffers.reserve(config.max_frames_in_flight);
        for (int i = 0; i < config.max_frames_in_flight; ++i) {
            uniform_buffers.push_back(VulkanUniformBuffer({
                .device = &device,
                .name = std::format("{} uniform buffer {}/{}", config.name, i + 1, uniform_buffers.size()),
                .size = sizeof(UniformBufferObject),
            }));
        }
    }

    void VulkanRenderer::do_commands(const std::function<void(const VulkanCommandBuffer&)>& on_record_commands) const {
        VulkanCommandBuffer command_buffer = transient_command_pool.allocate_command_buffer();

        if (command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) != VK_SUCCESS) {
            ST_THROW("Could not begin command buffer to set vertex buffer data");
        }

        on_record_commands(command_buffer);

        if (command_buffer.end() != VK_SUCCESS) {
            ST_THROW("Could not end command buffer to set vertex buffer data");
        }

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = (VkCommandBuffer*) &command_buffer;

        if (graphics_queue.submit(submit_info) != VK_SUCCESS) {
            ST_THROW("Could not submit command buffer to graphics queue to set vertex buffer data");
        }

        if (device.wait_until_queue_idle(graphics_queue) != VK_SUCCESS) {
            ST_THROW("Could not wait for graphics queue to be idle to set vertex buffer data");
        }

        transient_command_pool.free_command_buffer(command_buffer);
    }

    void VulkanRenderer::begin_command_buffer(const VulkanCommandBuffer& command_buffer) const {
        if (command_buffer.reset() != VK_SUCCESS) {
            ST_THROW("Could not reset command buffer");
        }
        if (command_buffer.begin() != VK_SUCCESS) {
            ST_THROW("Could not begin command buffer");
        }
        device.begin_cmd_label(command_buffer, "CommandBuffer");
    }

    void VulkanRenderer::end_command_buffer(const VulkanCommandBuffer& command_buffer) const {
        device.end_cmd_label(command_buffer);
        if (command_buffer.end() != VK_SUCCESS) {
            ST_THROW("Could not end command buffer");
        }
    }

    std::vector<VkDescriptorSetLayoutBinding> VulkanRenderer::get_descriptor_set_layout_bindings() const {
        std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings(1);

        descriptor_set_layout_bindings[0].binding = 0; // Specifies the binding used in the shader (i.e. `layout(binding = 0)`)
        descriptor_set_layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_set_layout_bindings[0].descriptorCount = 1;
        descriptor_set_layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Specifies in which shader stages the descriptor is going to be referenced.
        descriptor_set_layout_bindings[0].pImmutableSamplers = nullptr;

        return descriptor_set_layout_bindings;
    }
}
