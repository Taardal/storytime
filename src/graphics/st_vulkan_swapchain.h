#pragma once

#include "st_vulkan_command_buffer.h"
#include "st_vulkan_device.h"
#include "event/st_window_resized_event.h"
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
        std::vector<VkFence> in_flight_fences;
        std::vector<VkSemaphore> image_available_semaphores;
        std::vector<VkSemaphore> render_finished_semaphores;
        u32 current_image = 0;
        bool surface_resized = false;

    public:
        VulkanSwapchain(const Config& config);

        ~VulkanSwapchain();

        void recreate();

        VkRenderPass get_render_pass() const;

        const VkExtent2D& get_image_extent() const;

        void begin_frame();

        void end_frame(VkCommandBuffer command_buffer);

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

        void create_sync_objects();

        void destroy_sync_objects() const;

        void subscribe_to_events();

        void unsubscribe_from_events();

        void on_window_resized_event(const WindowResizedEvent& event);

    public:
        void begin_render_pass(const VulkanCommandBuffer& command_buffer) const;

        void end_render_pass(const VulkanCommandBuffer& command_buffer) const;

        void set_viewport(const VulkanCommandBuffer& command_buffer) const;

        void set_scissor(const VulkanCommandBuffer& command_buffer) const;
    };
}
