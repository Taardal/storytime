#pragma once

#include "st_vulkan_command_buffer.h"
#include "st_vulkan_command_pool.h"
#include "st_vulkan_device.h"
#include "st_vulkan_graphics_pipeline.h"
#include "st_vulkan_physical_device.h"
#include "st_vulkan_swapchain.h"
#include "event/st_window_resized_event.h"
#include "graphics/st_vulkan_instance.h"
#include "system/st_dispatcher.h"
#include "window/st_window.h"

namespace Storytime {
    struct VulkanRendererConfig {
        Dispatcher* dispatcher = nullptr;
        Window* window = nullptr;
        std::string name = "";
        u32 max_frames_in_flight = 0;
        bool validation_layers_enabled = false;
    };

    class VulkanRenderer {
    public:
        typedef VulkanRendererConfig Config;

    private:
        Config config;
        VulkanInstance instance;
        VulkanPhysicalDevice physical_device;
        VulkanDevice device;
        VulkanSwapchain swapchain;
        VulkanGraphicsPipeline graphics_pipeline;
        VulkanCommandPool command_pool;
        std::vector<VkCommandBuffer> command_buffers;
        std::vector<VkFence> in_flight_fences;
        std::vector<VkSemaphore> image_available_semaphores;
        std::vector<VkSemaphore> render_finished_semaphores;
        u32 current_frame = 0;
        bool surface_resized = false;

    public:
        VulkanRenderer(const Config& config);

        ~VulkanRenderer();

        void begin_frame();

        void end_frame();

        void render();

        // void render_quad(const Quad& quad);
        //
        // void render_quad(const Quad& quad, const std::array<TextureCoordinate, 4>& texture_coordinates);

    private:
        void create_command_buffers();

        void create_sync_objects();

        void destroy_sync_objects() const;

        void subscribe_to_events();

        void unsubscribe_from_events();

        void on_window_resized_event(const WindowResizedEvent& event);

        void recreate_swapchain();
    };
}