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
              .min_required_image_count = config.max_frames_in_flight,
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
        create_command_buffers();
        create_sync_objects();
        subscribe_to_events();
    }

    VulkanRenderer::~VulkanRenderer() {
        unsubscribe_from_events();
        destroy_sync_objects();
        // Command buffers are destroyed when the command pool is destroyed.
    }

    void VulkanRenderer::begin_frame() {
        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame);
        VkFence in_flight_fence = in_flight_fences.at(current_frame);
        VkSemaphore image_available_semaphore = image_available_semaphores.at(current_frame);

        //
        // Wait for the previous frame to finish.
        //

        VkBool32 wait_for_all_fences = VK_TRUE;
        u64 wait_for_fences_timeout = UINT64_MAX; // Wait forever until the fence is signaled.
        if (device.wait_for_fences(1, &in_flight_fence, wait_for_all_fences, wait_for_fences_timeout) != VK_SUCCESS) {
            ST_THROW("Could not wait for 'in flight' fence for frame [" << current_frame + 1 << "] / [" << config.max_frames_in_flight << "]");
        }

        //
        // Acquire an image from the swapchain to use for the current frame.
        //

        u64 next_image_timeout = UINT64_MAX; // Wait forever until an image becomes available.
        VkFence image_available_fence = VK_NULL_HANDLE; // No fences to signal when an image becomes available.
        VkResult next_image_result = swapchain.acquire_next_image(
            next_image_timeout,
            image_available_semaphore, // Signal that an image is available.
            image_available_fence
        );
        // VK_ERROR_OUT_OF_DATE_KHR: The swapchain has become incompatible with the surface and can no longer be used for rendering.
        if (next_image_result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreate_swapchain();
            return;
        }
        // VK_SUBOPTIMAL_KHR: The swapchain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
        if (next_image_result != VK_SUCCESS && next_image_result != VK_SUBOPTIMAL_KHR) {
            ST_THROW("Could not acquire swapchain image for frame [" << current_frame + 1 << "] / [" << config.max_frames_in_flight << "]");
        }

        // Delay resetting the fence until after we have successfully acquired an image, and we know for sure we will be submitting work with it.
        // Thus, if we return early, the fence is still signaled and vkWaitForFences won't deadlock the next time we use the same fence object.
        if (device.reset_fences(1, &in_flight_fence) != VK_SUCCESS) {
            ST_THROW("Could not reset 'in flight' fence for frame [" << current_frame + 1 << "] / [" << config.max_frames_in_flight << "]");
        }

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
        VulkanCommandBuffer command_buffer = command_buffers.at(current_frame);
        VkFence in_flight_fence = in_flight_fences.at(current_frame);
        VkSemaphore image_available_semaphore = image_available_semaphores.at(current_frame);
        VkSemaphore render_finished_semaphore = render_finished_semaphores.at(current_frame);

        VkDebugUtilsLabelEXT end_render_pass_label{};
        end_render_pass_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        end_render_pass_label.pLabelName = "End render pass";
        device.insert_cmd_label(command_buffer, end_render_pass_label);

        swapchain.end_render_pass(command_buffer);

        device.end_cmd_label(command_buffer);

        if (command_buffer.end() != VK_SUCCESS) {
            ST_THROW("Could not end command buffer [" << current_frame + 1 << "] / [" << command_buffers.size() << "]");
        }

        //
        // Submit the rendering commands to the graphics queue to perform the rendering.
        //

        VulkanQueue graphics_queue = device.get_graphics_queue();

        VkDebugUtilsLabelEXT graphis_queue_label{};
        graphis_queue_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        graphis_queue_label.pLabelName = "GraphicsQueue";

        constexpr VkPipelineStageFlags color_output_pipeline_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &image_available_semaphore; // Wait until an image is available.
        submit_info.pWaitDstStageMask = &color_output_pipeline_stage; // Wait at the color output pipeline stage.
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = (VkCommandBuffer*) &command_buffer;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &render_finished_semaphore; // Signal that the image is rendered and ready for presentation.

        VkDebugUtilsLabelEXT graphics_queue_label{};
        graphics_queue_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        graphics_queue_label.pLabelName = "GraphicsQueue";
        device.begin_queue_label(graphics_queue, graphics_queue_label);

        VkDebugUtilsLabelEXT submit_render_commands_label{};
        submit_render_commands_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        submit_render_commands_label.pLabelName = "Submit render commands";
        device.insert_queue_label(graphics_queue, submit_render_commands_label);

        if (graphics_queue.submit(submit_info, in_flight_fence) != VK_SUCCESS) {
            ST_THROW("Could not submit render commands to graphics queue");
        }

        device.end_queue_label(graphics_queue);

        //
        // Present the rendered swapchain image to the surface (screen).
        //

        VkResult present_result = swapchain.present(render_finished_semaphore);

        // VK_ERROR_OUT_OF_DATE_KHR: The swapchain has become incompatible with the surface and can no longer be used for rendering.
        // VK_SUBOPTIMAL_KHR: The swapchain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
        if (present_result == VK_ERROR_OUT_OF_DATE_KHR || present_result == VK_SUBOPTIMAL_KHR || surface_resized) {
            surface_resized = false;
            recreate_swapchain();
        } else if (present_result != VK_SUCCESS) {
            ST_THROW("Could not present swapchain image to the surface");
        }

        current_frame = (current_frame + 1) % config.max_frames_in_flight;
    }

    void VulkanRenderer::render() {
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

    void VulkanRenderer::create_command_buffers() {
        command_buffers.resize(config.max_frames_in_flight);
        command_pool.allocate_command_buffers(command_buffers.size(), command_buffers.data());

        for (u32 i = 0; i < command_buffers.size(); ++i) {
            std::string command_buffer_name = std::format("{} command buffer {}/{}", config.name, i + 1, command_buffers.size());
            if (device.set_object_name(command_buffers.at(i), VK_OBJECT_TYPE_COMMAND_BUFFER, command_buffer_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set command buffer name [" << command_buffer_name << "]");
            }
        }
    }

    void VulkanRenderer::create_sync_objects() {
        u32 image_count = config.max_frames_in_flight;

        image_available_semaphores.resize(image_count);
        render_finished_semaphores.resize(image_count);
        in_flight_fences.resize(image_count);

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

        for (u32 i = 0; i < image_count; i++) {
            if (device.create_semaphore(semaphore_create_info, &image_available_semaphores[i]) != VK_SUCCESS) {
                ST_THROW("Could not create 'image available' semaphore");
            }

            if (device.create_semaphore(semaphore_create_info, &render_finished_semaphores[i]) != VK_SUCCESS) {
                ST_THROW("Could not create 'render finished' semaphore");
            }

            if (device.create_fence(fence_create_info, &in_flight_fences[i]) != VK_SUCCESS) {
                ST_THROW("Could not create 'in flight' fence");
            }

            std::string image_available_semaphore_name = std::format("{} 'image available' semaphore {}/{}", config.name.c_str(), i + 1, image_count);
            if (device.set_object_name(image_available_semaphores[i], VK_OBJECT_TYPE_SEMAPHORE, image_available_semaphore_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set 'image available' semaphore name [" << image_available_semaphore_name << "]");
            }

            std::string render_finished_semaphore_name = std::format("{} 'render finished' semaphore {}/{}", config.name.c_str(), i + 1, image_count);
            if (device.set_object_name(render_finished_semaphores[i], VK_OBJECT_TYPE_SEMAPHORE, render_finished_semaphore_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set 'render finished' semaphore name [" << render_finished_semaphore_name << "]");
            }

            std::string in_flight_fence_name = std::format("{} 'in flight' fence {}/{}", config.name.c_str(), i + 1, image_count);
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

    void VulkanRenderer::subscribe_to_events() {
        config.dispatcher->subscribe<WindowResizedEvent, &VulkanRenderer::on_window_resized_event>(this);
    }

    void VulkanRenderer::unsubscribe_from_events() {
        config.dispatcher->unsubscribe<WindowResizedEvent, &VulkanRenderer::on_window_resized_event>(this);
    }

    void VulkanRenderer::on_window_resized_event(const WindowResizedEvent&) {
        surface_resized = true;
    }

    void VulkanRenderer::recreate_swapchain() {
        config.window->wait_until_not_minimized();
        if (device.wait_until_idle() != VK_SUCCESS) {
            ST_THROW("Could not wait until Vulkan device is idle to recreate swapchain");
        }
        swapchain.recreate();
    }
}
