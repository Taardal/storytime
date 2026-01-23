#include "st_vulkan_swapchain.h"

#include "graphics/st_vulkan_queue.h"

namespace Storytime {
    VulkanSwapchain::VulkanSwapchain(const Config& config) : config(config.assert_valid()) {
        create_swapchain();
        create_image_views();
        create_depth_image();
        create_render_pass();
        create_framebuffers();
        subscribe_to_events();
    }

    VulkanSwapchain::~VulkanSwapchain() {
        unsubscribe_from_events();
        destroy_framebuffers();
        destroy_render_pass();
        destroy_depth_image();
        destroy_image_views();
        destroy_swapchain();
    }

    void VulkanSwapchain::recreate() {
        config.window.wait_until_not_minimized();

        ST_ASSERT_THROW_VK(
            config.device.wait_until_idle(),
            "Could not wait for device to become idle when recreating swapchain"
        );

        destroy_framebuffers();
        destroy_depth_image();
        destroy_image_views();
        destroy_swapchain();

        create_swapchain();
        create_image_views();
        create_depth_image();
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

    u32 VulkanSwapchain::get_image_count() const {
        return color_images.size();
    }

    bool VulkanSwapchain::acquire_frame(const Frame& frame) {
        VkFence in_flight_fence = frame.in_flight_fence;
        VkSemaphore image_available_semaphore = frame.image_available_semaphore;

        //
        // Wait for the previous frame to finish.
        //
        // NOTE! The fence for the first frame MUST have been created in the signaled state (VK_FENCE_CREATE_SIGNALED_BIT).
        //
        // When acquiring the first frame after initialization there are no previous frames to wait for, which means that the first call
        // to vkWaitForFences() will wait forever.
        //
        // This can be solved by creating the fence in the signaled state (VK_FENCE_CREATE_SIGNALED_BIT), so that the first call to
        // vkWaitForFences() returns immediately since the fence is already signaled.
        //

        VkBool32 wait_for_all_fences = VK_TRUE;
        u64 wait_for_fences_timeout = UINT64_MAX; // Wait forever until the fence is signaled.
        VkResult wait_for_fence_result = config.device.wait_for_fences(1, &in_flight_fence, wait_for_all_fences, wait_for_fences_timeout);
        if (wait_for_fence_result != VK_SUCCESS) {
            ST_THROW("Could not wait for 'in flight' fence: " << format_vk_result(wait_for_fence_result));
        }

        //
        // Acquire an image from the swapchain to use for the current frame.
        //

        u64 next_image_timeout = UINT64_MAX; // Wait forever until an image becomes available.
        VkFence image_available_fence = VK_NULL_HANDLE; // No fences to signal when an image becomes available.
        VkResult next_image_result = config.device.acquire_next_swapchain_image(
            swapchain,
            next_image_timeout,
            image_available_semaphore, // Signal that an image is available.
            image_available_fence,
            &image_index
        );

        // VK_ERROR_OUT_OF_DATE_KHR: The swapchain has become incompatible with the surface and can no longer be used for rendering.
        if (next_image_result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreate();
            return false;
        }

        // VK_SUBOPTIMAL_KHR: The swapchain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
        if (next_image_result != VK_SUCCESS && next_image_result != VK_SUBOPTIMAL_KHR) {
            ST_THROW("Could not acquire next image: " << format_vk_result(next_image_result));
        }

        // Delay resetting the 'in flight' fence until after we have successfully acquired an image, and we know for sure we will be submitting work with it.
        // Thus, if we return early, the fence is still signaled and vkWaitForFences won't deadlock the next time we use the same fence object.
        VkResult reset_fence_result = config.device.reset_fences(1, &in_flight_fence);
        if (reset_fence_result != VK_SUCCESS) {
            ST_THROW("Could not reset 'in flight' fence: " << format_vk_result(reset_fence_result));
        }

        return true;
    }

    void VulkanSwapchain::present_frame(const Frame& frame) {

        VkCommandBuffer command_buffer = frame.command_buffer;
        VkFence in_flight_fence = frame.in_flight_fence;
        VkSemaphore image_available_semaphore = frame.image_available_semaphore;
        VkSemaphore render_finished_semaphore = frame.render_finished_semaphore;
        VulkanQueue graphics_queue = frame.graphics_queue;
        VulkanQueue present_queue = frame.present_queue;

        //
        // Submit the render commands to the graphics queue to perform the rendering.
        //

        config.device.begin_queue_label(graphics_queue, "GraphicsQueue");

        constexpr VkPipelineStageFlags color_output_pipeline_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &image_available_semaphore; // Wait until an image is available.
        submit_info.pWaitDstStageMask = &color_output_pipeline_stage; // Wait at the color output pipeline stage.
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &render_finished_semaphore; // Signal that the rendering to the image is complete.

        config.device.insert_queue_label(graphics_queue, "Submit render commands");

        VkResult submit_result = graphics_queue.submit(submit_info, in_flight_fence);
        if (submit_result != VK_SUCCESS) {
            ST_THROW("Could not submit render commands to graphics queue: " << format_vk_result(submit_result));
        }

        config.device.end_queue_label(graphics_queue);

        //
        // Present the rendered image to the surface (screen).
        //

        config.device.begin_queue_label(present_queue, "PresentQueue");

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.pImageIndices = &image_index;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &swapchain;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &render_finished_semaphore; // Wait until the image is rendered.
        present_info.pResults = nullptr;

        config.device.insert_queue_label(present_queue, "Present swapchain image");

        VkResult present_result = present_queue.present(present_info);

        // VK_ERROR_OUT_OF_DATE_KHR: The swapchain has become incompatible with the surface and can no longer be used for rendering.
        // VK_SUBOPTIMAL_KHR: The swapchain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
        if (present_result == VK_ERROR_OUT_OF_DATE_KHR || present_result == VK_SUBOPTIMAL_KHR || surface_has_been_resized) {
            surface_has_been_resized = false;
            recreate();
        } else if (present_result != VK_SUCCESS) {
            ST_THROW("Could not present image to the surface: " << format_vk_result(present_result));
        }

        config.device.end_queue_label(present_queue);
    }

    void VulkanSwapchain::begin_render_pass(const VulkanCommandBuffer& command_buffer) const {

        config.device.insert_cmd_label(command_buffer, "Begin swapchain render pass");

        VkClearColorValue clear_color_value = {
            .float32 = {
                config.clear_color.r,
                config.clear_color.g,
                config.clear_color.b,
                config.clear_color.a
            }
        };

        VkClearDepthStencilValue clear_depth_stencil_value = {
            .depth = depth_far, // The initial value at each point in the depth buffer should be the furthest possible depth.
            .stencil = 0,
        };

        std::array<VkClearValue, 2> clear_values{};
        clear_values[0].color = clear_color_value;
        clear_values[1].depthStencil = clear_depth_stencil_value;

        VkRenderPassBeginInfo render_pass_begin_info{};
        render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_begin_info.renderPass = render_pass;
        render_pass_begin_info.framebuffer = framebuffers[image_index];
        render_pass_begin_info.renderArea.offset = {0, 0};
        render_pass_begin_info.renderArea.extent = image_extent;
        render_pass_begin_info.clearValueCount = (u32) clear_values.size();
        render_pass_begin_info.pClearValues = clear_values.data();

        command_buffer.begin_render_pass(render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        config.device.insert_cmd_label(command_buffer, "Set swapchain viewport");

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (f32) image_extent.width;
        viewport.height = (f32) image_extent.height;
        viewport.minDepth = depth_near;
        viewport.maxDepth = depth_far;

        command_buffer.set_viewport(viewport);

        config.device.insert_cmd_label(command_buffer, "Set swapchain scissor");

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = image_extent;

        command_buffer.set_scissor(scissor);
    }

    void VulkanSwapchain::end_render_pass(const VulkanCommandBuffer& command_buffer) const {
        config.device.insert_cmd_label(command_buffer, "End swapchain render pass");
        command_buffer.end_render_pass();
    }

    void VulkanSwapchain::create_swapchain() {
        const VulkanPhysicalDevice& physical_device = config.device.get_physical_device();

        std::vector<VkPresentModeKHR> present_modes;
        ST_ASSERT_THROW_VK(
            physical_device.get_present_modes(&present_modes),
            "Could not get present modes"
        );

        std::vector<VkSurfaceFormatKHR> surface_formats;
        ST_ASSERT_THROW_VK(
            physical_device.get_surface_formats(&surface_formats),
            "Could not get surface formats"
        );

        VkSurfaceCapabilitiesKHR surface_capabilities;
        ST_ASSERT_THROW_VK(
            physical_device.get_surface_capabilities(&surface_capabilities),
            "Could not get surface capabilities"
        );

        present_mode = find_present_mode(present_modes);
        surface_format = find_surface_format(surface_formats);
        image_extent = find_image_extent(surface_capabilities);

        VkSwapchainCreateInfoKHR swapchain_create_info{};
        swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchain_create_info.minImageCount = find_min_image_count(surface_capabilities);
        swapchain_create_info.surface = config.surface;
        swapchain_create_info.imageFormat = surface_format.format;
        swapchain_create_info.imageColorSpace = surface_format.colorSpace;
        swapchain_create_info.imageExtent = image_extent;
        swapchain_create_info.imageArrayLayers = 1;
        swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchain_create_info.preTransform = surface_capabilities.currentTransform;
        swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_create_info.presentMode = present_mode;
        swapchain_create_info.clipped = VK_TRUE;
        swapchain_create_info.oldSwapchain = nullptr;

        u32 graphics_queue_family_index = physical_device.get_graphics_queue_family_index();
        u32 present_queue_family_index = physical_device.get_present_queue_family_index();

        if (graphics_queue_family_index != present_queue_family_index) {
            std::vector<u32> queue_families = {
                graphics_queue_family_index,
                present_queue_family_index,
            };
            swapchain_create_info.pQueueFamilyIndices = queue_families.data();
            swapchain_create_info.queueFamilyIndexCount = queue_families.size();
            swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        } else {
            swapchain_create_info.pQueueFamilyIndices = nullptr;
            swapchain_create_info.queueFamilyIndexCount = 0;
            swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        ST_ASSERT_THROW_VK(
            config.device.create_swapchain(swapchain_create_info, &swapchain, config.name),
            "Could not create swapchain"
        );

        ST_ASSERT_THROW_VK(
            config.device.get_swapchain_images(swapchain, &color_images),
            "Could not get swapchain images"
        );
    }

    void VulkanSwapchain::destroy_swapchain() const {
        config.device.destroy_swapchain(swapchain);
    }

    VkSurfaceFormatKHR VulkanSwapchain::find_surface_format(const std::vector<VkSurfaceFormatKHR>& surface_formats) const {
        for (const auto& surface_format : surface_formats) {
            if (surface_format.format == VK_FORMAT_B8G8R8A8_SRGB && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return surface_format;
            }
        }
        return surface_formats[0];
    }

    //
    // [VK_PRESENT_MODE_IMMEDIATE_KHR]
    // The simplest and fastest mode. The image is presented immediately, without waiting for a vertical blank. This
    // often results in tearing, but it's the best choice for applications that don't care about tearing or are using
    // other methods for synchronization.
    //
    // [VK_PRESENT_MODE_MAILBOX_KHR]
    // Replaces the image in the queue with the newest one if the queue is full, instead of waiting. This can achieve
    // lower latency compared to FIFO modes while still avoiding tearing. However, it is not always supported by all
    // hardware.
    //
    // [VK_PRESENT_MODE_FIFO_KHR]
    // The standard "vertical sync" mode. The application waits for the display to refresh and presents a new frame
    // only when there is a slot in the queue. This completely prevents screen tearing but can cause stuttering or
    // higher latency if the application cannot render frames at the same rate as the display's refresh rate.
    //
    // [VK_PRESENT_MODE_FIFO_RELAXED_KHR]
    // Similar to FIFO, but if the application is late and the queue was empty at the last vertical blank, it will
    // display the frame immediately instead of waiting for the next vertical blank. This can reduce stuttering caused
    // by occasional frame rate drops, but it may result in tearing during those instances.
    //
    VkPresentModeKHR VulkanSwapchain::find_present_mode(const std::vector<VkPresentModeKHR>& present_modes) const {
        if (!config.vsync_enabled) {
            return VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
        for (const VkPresentModeKHR& present_mode : present_modes) {
            if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return present_mode;
            }
        }
        for (const VkPresentModeKHR& present_mode : present_modes) {
            if (present_mode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
                return present_mode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapchain::find_image_extent(const VkSurfaceCapabilitiesKHR& surface_capabilities) const {
        bool extent_must_match_window_size = surface_capabilities.currentExtent.width != std::numeric_limits<u32>::max();
        if (extent_must_match_window_size) {
            return surface_capabilities.currentExtent;
        }

        const VkExtent2D& min_image_extent = surface_capabilities.minImageExtent;
        const VkExtent2D& max_image_extent = surface_capabilities.maxImageExtent;

        WindowSize window_size_px = config.window.get_size_in_pixels();
        u32 width = std::clamp((u32) window_size_px.width, min_image_extent.width, max_image_extent.width);
        u32 height = std::clamp((u32) window_size_px.height, min_image_extent.height, max_image_extent.height);

        return { width, height };
    }

    u32 VulkanSwapchain::find_min_image_count(const VkSurfaceCapabilitiesKHR& surface_capabilities) const {
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
        u32 image_count = color_images.size();
        color_image_views.resize(image_count);

        for (u32 i = 0; i < image_count; ++i) {
            VkImageViewCreateInfo image_view_create_info{};
            image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            image_view_create_info.image = color_images[i];
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

            std::string image_view_name = std::format("{} image view {}/{}", config.name, i + 1, image_count);

            ST_ASSERT_THROW_VK(
                config.device.create_image_view(image_view_create_info, &color_image_views[i], image_view_name),
                "Could not create swapchain image view [" << image_view_name << "]"
            );
        }
    }

    void VulkanSwapchain::destroy_image_views() const {
        for (VkImageView image_view : color_image_views) {
            config.device.destroy_image_view(image_view);
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

        VkAttachmentDescription depth_attachment_description{};
        depth_attachment_description.format = depth_image->get_format();
        depth_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
        depth_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depth_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // Don't care about storing the depth data because it will not be used after rendering has finished.
        depth_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depth_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depth_attachment_description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        std::array<VkAttachmentDescription, 2> attachment_descriptions = {
            color_attachment_description,
            depth_attachment_description
        };

        VkAttachmentReference color_attachment_reference{};
        color_attachment_reference.attachment = 0;
        color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depth_attachment_reference{};
        depth_attachment_reference.attachment = 1;
        depth_attachment_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass_description{};
        subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass_description.colorAttachmentCount = 1;
        subpass_description.pColorAttachments = &color_attachment_reference;
        subpass_description.pDepthStencilAttachment = &depth_attachment_reference;

        VkSubpassDependency subpass_dependency{};
        subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpass_dependency.dstSubpass = 0;
        subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpass_dependency.srcAccessMask = 0;
        subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo render_pass_create_info{};
        render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_create_info.attachmentCount = (u32) attachment_descriptions.size();
        render_pass_create_info.pAttachments = attachment_descriptions.data();
        render_pass_create_info.subpassCount = 1;
        render_pass_create_info.pSubpasses = &subpass_description;
        render_pass_create_info.dependencyCount = 1;
        render_pass_create_info.pDependencies = &subpass_dependency;

        std::string render_pass_name = std::format("{} render pass", config.name);

        ST_ASSERT_THROW_VK(
            config.device.create_render_pass(render_pass_create_info, &render_pass, render_pass_name),
            "Could not create swapchain render pass [" << render_pass_name << "]"
        );
    }

    void VulkanSwapchain::destroy_render_pass() const {
        config.device.destroy_render_pass(render_pass);
    }

    void VulkanSwapchain::create_framebuffers() {
        u32 image_count = color_images.size();
        framebuffers.resize(image_count);

        for (u32 i = 0; i < image_count; i++) {
            ST_ASSERT_IN_BOUNDS(i, color_image_views);
            VkImageView color_attachment = color_image_views.at(i);
            ST_ASSERT_NOT_NULL(color_attachment);

            VkImageView depth_attachment = depth_image->get_view();
            ST_ASSERT_NOT_NULL(depth_attachment);

            std::array<VkImageView, 2> attachments = {
                color_attachment,
                depth_attachment,
            };

            VkFramebufferCreateInfo framebuffer_create_info{};
            framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_create_info.renderPass = render_pass;
            framebuffer_create_info.attachmentCount = (u32) attachments.size();
            framebuffer_create_info.pAttachments = attachments.data();
            framebuffer_create_info.width = image_extent.width;
            framebuffer_create_info.height = image_extent.height;
            framebuffer_create_info.layers = 1;

            std::string framebuffer_name = std::format("{} framebuffer {}/{}", config.name, i + 1, image_count);

            ST_ASSERT_THROW_VK(
                config.device.create_framebuffer(framebuffer_create_info, &framebuffers[i], framebuffer_name),
                "Could not create swapchain framebuffer [" << framebuffer_name << "]"
            );
        }
    }

    void VulkanSwapchain::destroy_framebuffers() const {
        for (VkFramebuffer framebuffer : framebuffers) {
            config.device.destroy_framebuffer(framebuffer);
        }
    }

    void VulkanSwapchain::create_depth_image() {
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;

        const std::vector<VkFormat> format_candidates = {
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT
        };
        VkFormatFeatureFlags features = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
        VkFormat format = config.device.get_physical_device().find_supported_format(format_candidates, tiling, features);

        VkImageAspectFlags aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
        bool format_has_stencil_component = format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
        if (format_has_stencil_component) {
            aspect |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }

        // This must be created on the heap so it can be destroyed at the correct time to facilitate swap chain recreation.
        depth_image = ST_NEW VulkanImage({
            .name = std::format("{} depth image", config.name),
            .device = &config.device,
            .width = image_extent.width,
            .height = image_extent.height,
            .format = format,
            .tiling = tiling,
            .aspect = aspect,
            .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        });

        // We don't need to explicitly transition the layout of the image to VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL here
        // because it will be done by the render pass. The transition is specified in the depth attachment's VkAttachmentDescription object.
        // ```
        // depth_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        // depth_attachment_description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        // ```
    }

    void VulkanSwapchain::destroy_depth_image() const {
        delete depth_image;
    }

    void VulkanSwapchain::subscribe_to_events() {
        config.dispatcher.subscribe<WindowResizedEvent, &VulkanSwapchain::on_window_resized_event>(this);
    }

    void VulkanSwapchain::unsubscribe_from_events() {
        config.dispatcher.unsubscribe<WindowResizedEvent, &VulkanSwapchain::on_window_resized_event>(this);
    }

    void VulkanSwapchain::on_window_resized_event(const WindowResizedEvent&) {
        surface_has_been_resized = true;
    }
}
