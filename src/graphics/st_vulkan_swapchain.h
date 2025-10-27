#pragma once

#include "event/st_window_resized_event.h"
#include "graphics/st_vulkan_command_buffer.h"
#include "graphics/st_vulkan_device.h"
#include "graphics/st_frame.h"
#include "graphics/st_vulkan_image.h"
#include "system/st_dispatcher.h"
#include "window/st_window.h"

namespace Storytime {
    struct VulkanSwapchainConfig {
        std::string name = "SwapChain";
        Dispatcher* dispatcher = nullptr;
        Window* window = nullptr;
        VulkanDevice* device = nullptr;
        VkSurfaceKHR surface = nullptr;

        const VulkanSwapchainConfig& assert_valid() const {
            ST_ASSERT_NOT_NULL(dispatcher);
            ST_ASSERT_NOT_NULL(window);
            ST_ASSERT_NOT_NULL(device);
            ST_ASSERT_NOT_NULL(surface);
            return *this;
        }
    };

    class VulkanSwapchain {
    public:
        typedef VulkanSwapchainConfig Config;

    private:
        static constexpr f32 depth_near = 0.0f;
        static constexpr f32 depth_far = 1.0f;

    private:
        Config config{};
        VkSwapchainKHR swapchain = nullptr;
        VkSurfaceFormatKHR surface_format{};
        VkPresentModeKHR present_mode = VK_PRESENT_MODE_MAX_ENUM_KHR;
        VkExtent2D image_extent{};
        std::vector<VkImage> color_images{};
        std::vector<VkImageView> color_image_views{};
        VulkanImage* depth_image = nullptr;
        VkRenderPass render_pass = nullptr;
        std::vector<VkFramebuffer> framebuffers{};
        std::vector<VkFence> in_flight_fences{};
        std::vector<VkSemaphore> image_available_semaphores{};
        std::vector<VkSemaphore> render_finished_semaphores{};
        u32 image_index = 0;
        bool surface_has_been_resized = false;

    public:
        VulkanSwapchain(const Config& config);

        ~VulkanSwapchain();

        void recreate();

        operator VkSwapchainKHR() const;

        VkRenderPass get_render_pass() const;

        const VkExtent2D& get_image_extent() const;

        bool acquire_frame(const Frame& frame);

        void present_frame(const Frame& frame);

        void begin_render_pass(const VulkanCommandBuffer& command_buffer) const;

        void end_render_pass(const VulkanCommandBuffer& command_buffer) const;

    private:
        void create_swapchain();

        void destroy_swapchain() const;

        VkSurfaceFormatKHR find_surface_format(const std::vector<VkSurfaceFormatKHR>& surface_formats) const;

        VkPresentModeKHR find_present_mode(const std::vector<VkPresentModeKHR>& present_modes) const;

        VkExtent2D find_image_extent(const VkSurfaceCapabilitiesKHR& surface_capabilities) const;

        u32 find_min_image_count(const VkSurfaceCapabilitiesKHR& surface_capabilities) const;

        void create_image_views();

        void destroy_image_views() const;

        void create_depth_image();

        void destroy_depth_image() const;

        void create_render_pass();

        void destroy_render_pass() const;

        void create_framebuffers();

        void destroy_framebuffers() const;

        void subscribe_to_events();

        void unsubscribe_from_events();

        void on_window_resized_event(const WindowResizedEvent& event);
    };
}
