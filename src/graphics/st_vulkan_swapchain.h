#pragma once

#include "graphics/st_vulkan_command_buffer.h"
#include "graphics/st_vulkan_device.h"
#include "graphics/st_vulkan_queue.h"
#include "event/st_window_resized_event.h"
#include "system/st_dispatcher.h"
#include "window/st_window.h"

namespace Storytime {
    struct VulkanSwapchainConfig {
        Dispatcher* dispatcher = nullptr;
        Window* window = nullptr;
        VulkanPhysicalDevice* physical_device = nullptr;
        VulkanDevice* device = nullptr;
        VkSurfaceKHR surface = nullptr;
        std::string name = "SwapChain";
        u32 max_frames_in_flight = 0;
    };

    class VulkanSwapchain {
    public:
        typedef VulkanSwapchainConfig Config;

    private:
        Config config;
        VkSwapchainKHR swapchain = nullptr;
        VkSurfaceFormatKHR surface_format{};
        VkPresentModeKHR present_mode = VK_PRESENT_MODE_MAX_ENUM_KHR;
        VkExtent2D image_extent{};
        std::vector<VkImage> images;
        std::vector<VkImageView> image_views;
        std::vector<VkFramebuffer> framebuffers;
        VkRenderPass render_pass = nullptr;
        std::vector<VkFence> in_flight_fences;
        std::vector<VkSemaphore> image_available_semaphores;
        std::vector<VkSemaphore> render_finished_semaphores;
        VulkanQueue graphics_queue;
        VulkanQueue present_queue;
        u32 current_image_index = 0;
        bool surface_has_been_resized = false;

    public:
        VulkanSwapchain(const Config& config);

        ~VulkanSwapchain();

        void recreate();

        operator VkSwapchainKHR() const;

        VkRenderPass get_render_pass() const;

        const VkExtent2D& get_image_extent() const;

        bool acquire_frame(u32 frame_index);

        void begin_frame(const VulkanCommandBuffer& command_buffer) const;

        void end_frame(const VulkanCommandBuffer& command_buffer) const;

        void present_frame(u32 frame_index, VkCommandBuffer command_buffer);

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

        void initialize_queues();

        void subscribe_to_events();

        void unsubscribe_from_events();

        void on_window_resized_event(const WindowResizedEvent& event);
    };
}
