#pragma once

#include "st_vulkan_command_buffer.h"
#include "st_vulkan_command_pool.h"
#include "st_vulkan_device.h"
#include "st_vulkan_graphics_pipeline.h"
#include "st_vulkan_physical_device.h"
#include "st_vulkan_swapchain.h"
#include "graphics/st_vulkan_instance.h"
#include "system/st_dispatcher.h"
#include "window/st_window.h"

namespace Storytime {
    struct VulkanRendererConfig {
        Dispatcher* dispatcher = nullptr;
        Window* window = nullptr;
        std::string app_name = "";
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
        u32 current_frame = 0;

    public:
        VulkanRenderer(const Config& config);

        void begin_frame();

        void end_frame();

        void render();

        // void render_quad(const Quad& quad);
        //
        // void render_quad(const Quad& quad, const std::array<TextureCoordinate, 4>& texture_coordinates);
    };
}