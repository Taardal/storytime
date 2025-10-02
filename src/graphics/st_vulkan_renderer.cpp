#include "st_vulkan_renderer.h"

#include "st_quad_vertex.h"

namespace Storytime {

    const std::vector<QuadVertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
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
              .vertex_shader_path = ST_RES_DIR / std::filesystem::path("shaders/triangle.vert.spv"),
              .fragment_shader_path = ST_RES_DIR / std::filesystem::path("shaders/triangle.frag.spv"),
              .vertex_input_binding_description = QuadVertex::getBindingDescription(),
              .vertex_input_attribute_descriptions = QuadVertex::getAttributeDescriptions(),
          }),
          vertex_buffer({
              .device = &device,
              .name = std::format("{} vertex buffer", config.name),
              .size = sizeof(vertices[0]) * vertices.size(),
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
          })
    {
        allocate_command_buffers();
        initialize_queues();
        load_vertices();
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

        device.insert_cmd_label(command_buffer, "Draw");

        u32 vertex_count = vertices.size(); // Even though we don't have a vertex buffer, we technically still have 3 vertices to draw.
        u32 instance_count = 1; // Used for instanced rendering, use 1 if you're not doing that.
        u32 first_vertex = 0; // Used as an offset into the vertex buffer, defines the lowest value of gl_VertexIndex.
        u32 first_instance = 0; // Used as an offset for instanced rendering, defines the lowest value of gl_InstanceIndex.
        command_buffer.draw(vertex_count, instance_count, first_vertex, first_instance);
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

    void VulkanRenderer::load_vertices() const {
        VulkanCommandBuffer command_buffer = transient_command_pool.allocate_command_buffer();

        if (command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) != VK_SUCCESS) {
            ST_THROW("Could not begin command buffer to set vertex buffer data");
        }

        vertex_buffer.set_data(vertices.data(), command_buffer);

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
}
