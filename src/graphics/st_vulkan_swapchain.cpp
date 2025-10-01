#include "st_vulkan_swapchain.h"

#include "st_vulkan_queue.h"

namespace Storytime {
    VulkanSwapchain::VulkanSwapchain(const Config& config) : config(config) {
        create_swapchain();
        create_image_views();
        create_render_pass();
        create_framebuffers();
        create_sync_objects();
        initialize_queues();
        subscribe_to_events();
    }

    VulkanSwapchain::~VulkanSwapchain() {
        unsubscribe_from_events();
        destroy_sync_objects();
        destroy_framebuffers();
        destroy_render_pass();
        destroy_image_views();
        destroy_swapchain();
    }

    void VulkanSwapchain::recreate() {
        config.window->wait_until_not_minimized();

        if (config.device->wait_until_idle() != VK_SUCCESS) {
            ST_THROW("Could not wait until Vulkan device is idle to recreate swapchain");
        }

        destroy_framebuffers();
        destroy_image_views();
        destroy_swapchain();

        create_swapchain();
        create_image_views();
        create_framebuffers();
    }

    VulkanSwapchain::operator VkSwapchainKHR() const {
        return swapchain;
    }

    VkRenderPass VulkanSwapchain::get_render_pass() const {
        return render_pass;
    }

    const VkExtent2D& VulkanSwapchain::get_image_extent() const {
        return image_extent;
    }

    bool VulkanSwapchain::acquire_frame(u32 frame_index) {
        const VulkanDevice& device = *config.device;

        VkFence in_flight_fence = in_flight_fences.at(frame_index);
        VkSemaphore image_available_semaphore = image_available_semaphores.at(frame_index);

        //
        // Wait for the previous frame to finish.
        //

        VkBool32 wait_for_all_fences = VK_TRUE;
        u64 wait_for_fences_timeout = UINT64_MAX; // Wait forever until the fence is signaled.
        if (device.wait_for_fences(1, &in_flight_fence, wait_for_all_fences, wait_for_fences_timeout) != VK_SUCCESS) {
            ST_THROW("Could not wait for 'in flight' fence");
        }

        //
        // Acquire an image from the swapchain to use for the current frame.
        //

        u64 next_image_timeout = UINT64_MAX; // Wait forever until an image becomes available.
        VkFence image_available_fence = VK_NULL_HANDLE; // No fences to signal when an image becomes available.
        VkResult next_image_result = device.acquire_next_swapchain_image(
            swapchain,
            next_image_timeout,
            image_available_semaphore, // Signal that an image is available.
            image_available_fence,
            &current_image_index
        );

        // VK_ERROR_OUT_OF_DATE_KHR: The swapchain has become incompatible with the surface and can no longer be used for rendering.
        if (next_image_result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreate();
            return false;
        }

        // VK_SUBOPTIMAL_KHR: The swapchain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
        if (next_image_result != VK_SUCCESS && next_image_result != VK_SUBOPTIMAL_KHR) {
            ST_THROW("Could not acquire swapchain image");
        }

        // Delay resetting the 'in flight' fence until after we have successfully acquired an image, and we know for sure we will be submitting work with it.
        // Thus, if we return early, the fence is still signaled and vkWaitForFences won't deadlock the next time we use the same fence object.
        if (device.reset_fences(1, &in_flight_fence) != VK_SUCCESS) {
            ST_THROW("Could not reset 'in flight' fence");
        }

        return true;
    }

    void VulkanSwapchain::begin_frame(const VulkanCommandBuffer& command_buffer) const {
        const VulkanDevice& device = *config.device;

        device.insert_cmd_label(command_buffer, "Begin swapchain render pass");

        VkClearColorValue clear_color_value = {
            .float32 = {0.0f, 0.0f, 0.0f, 1.0f}
        };
        VkClearValue clear_color = {
            .color = clear_color_value
        };

        VkRenderPassBeginInfo render_pass_begin_info{};
        render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_begin_info.renderPass = render_pass;
        render_pass_begin_info.framebuffer = framebuffers[current_image_index];
        render_pass_begin_info.renderArea.offset = {0, 0};
        render_pass_begin_info.renderArea.extent = image_extent;
        render_pass_begin_info.clearValueCount = 1;
        render_pass_begin_info.pClearValues = &clear_color;

        command_buffer.begin_render_pass(render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        device.insert_cmd_label(command_buffer, "Set swapchain viewport");

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (f32) image_extent.width;
        viewport.height = (f32) image_extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        command_buffer.set_viewport(viewport);

        device.insert_cmd_label(command_buffer, "Set swapchain scissor");

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = image_extent;
        command_buffer.set_scissor(scissor);
    }

    void VulkanSwapchain::end_frame(const VulkanCommandBuffer& command_buffer) const {
        config.device->insert_cmd_label(command_buffer, "End render pass");
        command_buffer.end_render_pass();
    }

    void VulkanSwapchain::present_frame(u32 frame_index, VkCommandBuffer command_buffer) {
        const VulkanDevice& device = *config.device;

        VkFence in_flight_fence = in_flight_fences.at(frame_index);
        VkSemaphore image_available_semaphore = image_available_semaphores.at(frame_index);
        VkSemaphore render_finished_semaphore = render_finished_semaphores.at(frame_index);

        //
        // Submit the render commands to the graphics queue to perform the rendering.
        //

        device.begin_queue_label(graphics_queue, "GraphicsQueue");

        constexpr VkPipelineStageFlags color_output_pipeline_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &image_available_semaphore; // Wait until an image is available.
        submit_info.pWaitDstStageMask = &color_output_pipeline_stage; // Wait at the color output pipeline stage.
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &render_finished_semaphore; // Signal that the rendering to the image is complete.

        device.insert_queue_label(graphics_queue, "Submit render commands");

        if (graphics_queue.submit(submit_info, in_flight_fence) != VK_SUCCESS) {
            ST_THROW("Could not submit render commands to graphics queue");
        }

        device.end_queue_label(graphics_queue);

        //
        // Present the rendered image to the surface (screen).
        //

        device.begin_queue_label(present_queue, "PresentQueue");

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &render_finished_semaphore; // Wait until the image is rendered.
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &swapchain;
        present_info.pImageIndices = &current_image_index;
        present_info.pResults = nullptr;

        device.insert_queue_label(present_queue, "Present swapchain image");

        VkResult present_result = present_queue.present(present_info);

        device.end_queue_label(present_queue);

        // VK_ERROR_OUT_OF_DATE_KHR: The swapchain has become incompatible with the surface and can no longer be used for rendering.
        // VK_SUBOPTIMAL_KHR: The swapchain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
        if (present_result == VK_ERROR_OUT_OF_DATE_KHR || present_result == VK_SUBOPTIMAL_KHR || surface_has_been_resized) {
            surface_has_been_resized = false;
            recreate();
        } else if (present_result != VK_SUCCESS) {
            ST_THROW("Could not present swapchain image to the surface");
        }
    }

    void VulkanSwapchain::create_swapchain() {
        const VulkanPhysicalDevice& physical_device = *config.physical_device;
        const VulkanDevice& device = *config.device;

        surface_format = find_surface_format();
        present_mode = find_present_mode();
        image_extent = find_image_extent();

        VkSwapchainCreateInfoKHR swapchain_create_info{};
        swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchain_create_info.minImageCount = find_min_image_count();
        swapchain_create_info.surface = config.surface;
        swapchain_create_info.imageFormat = surface_format.format;
        swapchain_create_info.imageColorSpace = surface_format.colorSpace;
        swapchain_create_info.imageExtent = image_extent;
        swapchain_create_info.imageArrayLayers = 1;
        swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchain_create_info.preTransform = physical_device.get_surface_capabilities().currentTransform;
        swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_create_info.presentMode = present_mode;
        swapchain_create_info.clipped = VK_TRUE;
        swapchain_create_info.oldSwapchain = nullptr;

        const QueueFamilyIndices& queue_family_indices = physical_device.get_queue_family_indices();
        if (queue_family_indices.graphics_family != queue_family_indices.present_family) {
            std::vector<u32> queue_families = {
                queue_family_indices.graphics_family.value(),
                queue_family_indices.present_family.value()
            };
            swapchain_create_info.pQueueFamilyIndices = queue_families.data();
            swapchain_create_info.queueFamilyIndexCount = queue_families.size();
            swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        } else {
            swapchain_create_info.pQueueFamilyIndices = nullptr;
            swapchain_create_info.queueFamilyIndexCount = 0;
            swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        if (device.create_swapchain(swapchain_create_info, &swapchain) != VK_SUCCESS) {
            ST_THROW("Could not create swapchain");
        }

        if (device.set_object_name(swapchain, VK_OBJECT_TYPE_SWAPCHAIN_KHR, config.name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set swapchain name [" << config.name << "]");
        }

        if (device.get_swapchain_images(swapchain, &images) != VK_SUCCESS) {
            ST_THROW("Could not get swapchain images");
        }

        if (images.size() < config.max_frames_in_flight) {
            ST_THROW("Could not create swapchain with fewer images [" << images.size() << "] than frames in flight [" << config.max_frames_in_flight << "]");
        }
    }

    void VulkanSwapchain::destroy_swapchain() const {
        config.device->destroy_swapchain(swapchain);
    }

    VkSurfaceFormatKHR VulkanSwapchain::find_surface_format() const {
        const std::vector<VkSurfaceFormatKHR>& surface_formats = config.physical_device->get_surface_formats();
        for (const auto& surface_format : surface_formats) {
            if (surface_format.format == VK_FORMAT_B8G8R8A8_SRGB && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return surface_format;
            }
        }
        return surface_formats[0];
    }

    VkPresentModeKHR VulkanSwapchain::find_present_mode() const {
        for (const VkPresentModeKHR& present_mode : config.physical_device->get_present_modes()) {
            if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return present_mode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapchain::find_image_extent() const {
        const VkSurfaceCapabilitiesKHR& surface_capabilities = config.physical_device->get_surface_capabilities();

        bool extent_must_match_window_size = surface_capabilities.currentExtent.width != std::numeric_limits<u32>::max();
        if (extent_must_match_window_size) {
            return surface_capabilities.currentExtent;
        }

        const VkExtent2D& min_image_extent = surface_capabilities.minImageExtent;
        const VkExtent2D& max_image_extent = surface_capabilities.maxImageExtent;

        WindowSize window_size_px = config.window->get_size_in_pixels();
        u32 width = std::clamp((u32) window_size_px.width, min_image_extent.width, max_image_extent.width);
        u32 height = std::clamp((u32) window_size_px.height, min_image_extent.height, max_image_extent.height);

        return { width, height };
    }

    u32 VulkanSwapchain::find_min_image_count() const {
        const VkSurfaceCapabilitiesKHR& surface_capabilities = config.physical_device->get_surface_capabilities();

        u32 min_image_count = surface_capabilities.minImageCount;
        u32 max_image_count = surface_capabilities.maxImageCount;

        // Simply sticking to the surface capability minimum means that we may sometimes have to wait on the driver to complete internal
        // operations before we can acquire another image to render to. Therefore, it is recommended to request at least one more image
        // than the minimum.
        u32 image_count = min_image_count + 1;

        // We need to make sure to not exceed the maximum image count after adding 1 to the minimum.
        // For the surface capability maximum, `0` is a special value that means that there is no maximum.
        if (max_image_count > 0 && image_count > max_image_count) {
            image_count = max_image_count;
        }

        return image_count;
    }

    void VulkanSwapchain::create_image_views() {
        u32 image_count = images.size();
        image_views.resize(image_count);

        for (u32 i = 0; i < image_count; ++i) {
            VkImageViewCreateInfo image_view_create_info{};
            image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            image_view_create_info.image = images[i];
            image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            image_view_create_info.format = surface_format.format;
            image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            image_view_create_info.subresourceRange.baseMipLevel = 0;
            image_view_create_info.subresourceRange.levelCount = 1;
            image_view_create_info.subresourceRange.baseArrayLayer = 0;
            image_view_create_info.subresourceRange.layerCount = 1;

            if (config.device->create_image_view(image_view_create_info, &image_views[i]) != VK_SUCCESS) {
                ST_THROW("Could not create swapchain image view [" << i + 1 << "] / [" << image_count << "]");
            }

            std::string image_view_name = std::format("{} image view {}/{}", config.name, i + 1, image_count);
            if (config.device->set_object_name(image_views[i], VK_OBJECT_TYPE_IMAGE_VIEW, image_view_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set swapchain image view name [" << image_view_name << "]");
            }
        }
    }

    void VulkanSwapchain::destroy_image_views() const {
        for (VkImageView image_view : image_views) {
            config.device->destroy_image_view(image_view);
        }
    }

    void VulkanSwapchain::create_render_pass() {
        VkAttachmentDescription color_attachment_description{};
        color_attachment_description.format = surface_format.format;
        color_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_reference{};
        color_attachment_reference.attachment = 0;
        color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass_description{};
        subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass_description.colorAttachmentCount = 1;
        subpass_description.pColorAttachments = &color_attachment_reference;

        VkSubpassDependency subpass_dependency{};
        subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpass_dependency.dstSubpass = 0;
        subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpass_dependency.srcAccessMask = 0;
        subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo render_pass_create_info{};
        render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_create_info.attachmentCount = 1;
        render_pass_create_info.pAttachments = &color_attachment_description;
        render_pass_create_info.subpassCount = 1;
        render_pass_create_info.pSubpasses = &subpass_description;
        render_pass_create_info.dependencyCount = 1;
        render_pass_create_info.pDependencies = &subpass_dependency;

        if (config.device->create_render_pass(render_pass_create_info, &render_pass) != VK_SUCCESS) {
            ST_THROW("Could not create swapchain render pass");
        }

        std::string render_pass_name = std::format("{} render pass", config.name);
        if (config.device->set_object_name(render_pass, VK_OBJECT_TYPE_RENDER_PASS, render_pass_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set swapchain render pass name [" << render_pass_name << "]");
        }
    }

    void VulkanSwapchain::destroy_render_pass() const {
        config.device->destroy_render_pass(render_pass);
    }

    void VulkanSwapchain::create_framebuffers() {
        u32 image_count = images.size();
        framebuffers.resize(image_count);

        for (size_t i = 0; i < image_count; i++) {
            VkImageView attachments[] = {
                image_views[i]
            };

            VkFramebufferCreateInfo framebuffer_create_info{};
            framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_create_info.renderPass = render_pass;
            framebuffer_create_info.attachmentCount = 1;
            framebuffer_create_info.pAttachments = attachments;
            framebuffer_create_info.width = image_extent.width;
            framebuffer_create_info.height = image_extent.height;
            framebuffer_create_info.layers = 1;

            if (config.device->create_framebuffer(framebuffer_create_info, &framebuffers[i]) != VK_SUCCESS) {
                ST_THROW("Could not create swapchain framebuffer [" << i + 1 << " / " << image_count << "]");
            }

            std::string framebuffer_name = std::format("{} framebuffer {}/{}", config.name, i + 1, image_count);
            if (config.device->set_object_name(framebuffers[i], VK_OBJECT_TYPE_FRAMEBUFFER, framebuffer_name.c_str()) != VK_SUCCESS) {
                ST_THROW("Could not set swapchain framebuffer name [" << framebuffer_name << "]");
            }
        }
    }

    void VulkanSwapchain::destroy_framebuffers() const {
        for (VkFramebuffer framebuffer : framebuffers) {
            config.device->destroy_framebuffer(framebuffer);
        }
    }

    void VulkanSwapchain::initialize_queues() {
        const VulkanPhysicalDevice& physical_device = *config.physical_device;
        const VulkanDevice& device = *config.device;

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

    void VulkanSwapchain::create_sync_objects() {
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

    void VulkanSwapchain::destroy_sync_objects() const {
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

    void VulkanSwapchain::subscribe_to_events() {
        config.dispatcher->subscribe<WindowResizedEvent, &VulkanSwapchain::on_window_resized_event>(this);
    }

    void VulkanSwapchain::unsubscribe_from_events() {
        config.dispatcher->unsubscribe<WindowResizedEvent, &VulkanSwapchain::on_window_resized_event>(this);
    }

    void VulkanSwapchain::on_window_resized_event(const WindowResizedEvent&) {
        surface_has_been_resized = true;
    }
}
