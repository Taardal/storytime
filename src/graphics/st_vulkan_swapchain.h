#pragma once

#include "st_vulkan_command_buffer.h"
#include "st_vulkan_device.h"
#include "system/st_dispatcher.h"
#include "window/st_window.h"

namespace Storytime {
    struct VulkanSwapchainConfig {
        Dispatcher* dispatcher = nullptr;
        Window* window = nullptr;
        VulkanInstance* instance = nullptr;
        VulkanPhysicalDevice* physical_device = nullptr;
        VulkanDevice* device = nullptr;
        std::string name = "SwapChain";
        u32 min_required_image_count = 0;
    };

    class VulkanSwapchain {
    public:
        typedef VulkanSwapchainConfig Config;

    private:
        Config config;
        VkSwapchainKHR swapchain = nullptr;
        VkSurfaceFormatKHR surface_format;
        VkPresentModeKHR present_mode;
        VkExtent2D image_extent;
        std::vector<VkImage> images;
        std::vector<VkImageView> image_views;
        std::vector<VkFramebuffer> framebuffers;
        VkRenderPass render_pass = nullptr;
        u32 current_image = 0;

    public:
        VulkanSwapchain(const Config& config);

        ~VulkanSwapchain();

        void recreate();

        VkRenderPass get_render_pass() const;

        const VkExtent2D& get_image_extent() const;

        void begin_frame();

        void end_frame(VkCommandBuffer command_buffer);

        void begin_render_pass(const VulkanCommandBuffer& command_buffer) const;

        void end_render_pass(const VulkanCommandBuffer& command_buffer) const;

        void set_viewport(const VulkanCommandBuffer& command_buffer) const;

        void set_scissor(const VulkanCommandBuffer& command_buffer) const;

        struct AcquireNextImageConfig {
            u64 timeout;
            VkSemaphore semaphore;
            VkFence fence;
        };

        VkResult acquire_next_image(const AcquireNextImageConfig& config);

        VkResult acquire_next_image(u64 timeout, VkSemaphore semaphore, VkFence fence);

        VkResult present(VkSemaphore wait_semaphore) const;

    private:
        void create_swapchain();

        void destroy_swapchain() const;

        VkSurfaceFormatKHR find_surface_format() const;

        VkPresentModeKHR find_present_mode() const;

        VkExtent2D find_image_extent() const;

        u32 find_min_image_count() const;

        void create_image_views();

        void destroy_image_views() const;

        void create_render_pass();

        void destroy_render_pass() const;

        void create_framebuffers();

        void destroy_framebuffers() const;
    };
}
