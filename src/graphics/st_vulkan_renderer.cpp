#include "st_vulkan_renderer.h"

namespace Storytime {
    VulkanRenderer::VulkanRenderer(const Config& config)
        : config(config),
          instance({
              .window = config.window,
              .app_name = config.app_name,
              .engine_name = std::format("{} engine", config.app_name),
              .validation_layers_enabled = config.validation_layers_enabled,
          }),
          physical_device({
              .instance = &instance,
          }),
          device({
              .physical_device = &physical_device,
              .name = std::format("{} device", config.app_name)
          }),
          swapchain({
              .dispatcher = config.dispatcher,
              .window = config.window,
              .instance = &instance,
              .physical_device = &physical_device,
              .device = &device,
              .name = std::format("{} swapchain", config.app_name)
          }),
          graphics_pipeline({
              .device = &device,
              .swapchain = &swapchain,
              .name = std::format("{} graphics pipeline", config.app_name),
              .vertex_shader_path = ST_RES_DIR / std::filesystem::path("shaders/triangle.vert.spv"),
              .fragment_shader_path = ST_RES_DIR / std::filesystem::path("shaders/triangle.frag.spv"),
          }),
          command_pool({
              .physical_device = &physical_device,
              .device = &device,
              .name = std::format("{} command pool", config.app_name),
          }),
          command_buffers(command_pool.allocate_command_buffers(config.max_frames_in_flight))
    {
    }

    void VulkanRenderer::begin_frame() {
        std::cout  << "BEGIN [" << current_frame + 1 << "] / [" << command_buffers.size() << "]" << std::endl;
        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame);

        swapchain.begin_frame();

        if (command_buffer.reset() != VK_SUCCESS) {
            ST_THROW("Could not reset command buffer [" << current_frame + 1 << "] / [" << command_buffers.size() << "]");
        }
        if (command_buffer.begin() != VK_SUCCESS) {
            ST_THROW("Could not begin command buffer [" << current_frame + 1 << "] / [" << command_buffers.size() << "]");
        }

        VkDebugUtilsLabelEXT command_buffer_label{};
        command_buffer_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        command_buffer_label.pLabelName = "CommandBuffer";
        device.begin_cmd_label(command_buffer, command_buffer_label);

        // VkDebugUtilsLabelEXT begin_render_pass_label{};
        // begin_render_pass_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        // begin_render_pass_label.pLabelName = "Begin render pass";
        // device.insert_cmd_label(command_buffer, begin_render_pass_label);

        swapchain.begin_render_pass(command_buffer);

        VkDebugUtilsLabelEXT bind_pipeline_label{};
        bind_pipeline_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        bind_pipeline_label.pLabelName = "Bind pipeline";
        device.insert_cmd_label(command_buffer, bind_pipeline_label);

        command_buffer.bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

        // VkDebugUtilsLabelEXT set_viewport_label{};
        // set_viewport_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        // set_viewport_label.pLabelName = "Set viewport";
        // device.insert_cmd_label(command_buffer, set_viewport_label);

        swapchain.set_viewport(command_buffer);

        // VkDebugUtilsLabelEXT set_scissor_label{};
        // set_scissor_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        // set_scissor_label.pLabelName = "Set scissor";
        // device.insert_cmd_label(command_buffer, set_scissor_label);

        swapchain.set_scissor(command_buffer);
    }

    void VulkanRenderer::end_frame() {
        std::cout  << "END [" << current_frame + 1 << "] / [" << command_buffers.size() << "]" << std::endl;
        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame);

        VkDebugUtilsLabelEXT end_render_pass_label{};
        end_render_pass_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        end_render_pass_label.pLabelName = "End render pass";
        device.insert_cmd_label(command_buffer, end_render_pass_label);

        swapchain.end_render_pass(command_buffer);

        device.end_cmd_label(command_buffer);

        if (command_buffer.end() != VK_SUCCESS) {
            ST_THROW("Could not end command buffer [" << current_frame + 1 << "] / [" << command_buffers.size() << "]");
        }

        swapchain.end_frame(command_buffer);
        current_frame = (current_frame + 1) % 2;
    }

    void VulkanRenderer::render() {
        std::cout  << "RENDER [" << current_frame + 1 << "] / [" << command_buffers.size() << "]" << std::endl;
        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame);

        VkDebugUtilsLabelEXT draw_label{};
        draw_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        draw_label.pLabelName = "Draw";
        device.insert_cmd_label(command_buffer, draw_label);

        u32 vertex_count = 3; // Even though we don't have a vertex buffer, we technically still have 3 vertices to draw.
        u32 instance_count = 1; // Used for instanced rendering, use 1 if you're not doing that.
        u32 first_vertex = 0; // Used as an offset into the vertex buffer, defines the lowest value of gl_VertexIndex.
        u32 first_instance = 0; // Used as an offset for instanced rendering, defines the lowest value of gl_InstanceIndex.
        command_buffer.draw(vertex_count, instance_count, first_vertex, first_instance);
    }
}
