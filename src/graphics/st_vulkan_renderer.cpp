#include "st_vulkan_renderer.h"

#include "st_vulkan_queue.h"

namespace Storytime {
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
              .instance = &instance,
              .physical_device = &physical_device,
              .device = &device,
              .name = std::format("{} swapchain", config.name),
              .max_frames_in_flight = config.max_frames_in_flight,
          }),
          graphics_pipeline({
              .device = &device,
              .swapchain = &swapchain,
              .name = std::format("{} graphics pipeline", config.name),
              .vertex_shader_path = ST_RES_DIR / std::filesystem::path("shaders/triangle.vert.spv"),
              .fragment_shader_path = ST_RES_DIR / std::filesystem::path("shaders/triangle.frag.spv"),
          }),
          command_pool({
              .physical_device = &physical_device,
              .device = &device,
              .name = std::format("{} command pool", config.name),
          })
    {
        allocate_command_buffers();
    }

    void VulkanRenderer::begin_frame() {
        if (!swapchain.acquire_frame(current_frame_index)) {
            return;
        }

        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame_index);
        begin_command_buffer(command_buffer);

        swapchain.begin_frame(command_buffer);

        device.insert_cmd_label(command_buffer, "Bind pipeline");
        command_buffer.bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);
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

        device.insert_cmd_label(command_buffer, "Draw");

        u32 vertex_count = 3; // Even though we don't have a vertex buffer, we technically still have 3 vertices to draw.
        u32 instance_count = 1; // Used for instanced rendering, use 1 if you're not doing that.
        u32 first_vertex = 0; // Used as an offset into the vertex buffer, defines the lowest value of gl_VertexIndex.
        u32 first_instance = 0; // Used as an offset for instanced rendering, defines the lowest value of gl_InstanceIndex.
        command_buffer.draw(vertex_count, instance_count, first_vertex, first_instance);
    }

    void VulkanRenderer::allocate_command_buffers() {
        command_buffers.resize(config.max_frames_in_flight);
        command_pool.allocate_command_buffers(command_buffers.size(), command_buffers.data());

        for (u32 i = 0; i < command_buffers.size(); ++i) {
            std::string command_buffer_name = std::format("{} command buffer {}/{}", config.name, i + 1, command_buffers.size());
            if (device.set_object_name(command_buffers.at(i), VK_OBJECT_TYPE_COMMAND_BUFFER, command_buffer_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set command buffer name [" << command_buffer_name << "]");
            }
        }
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
