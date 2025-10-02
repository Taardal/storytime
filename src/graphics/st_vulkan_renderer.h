#pragma once

#include "st_vulkan_command_pool.h"
#include "st_vulkan_device.h"
#include "st_vulkan_graphics_pipeline.h"
#include "st_vulkan_index_buffer.h"
#include "st_vulkan_physical_device.h"
#include "st_vulkan_swapchain.h"
#include "st_vulkan_vertex_buffer.h"
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
        VulkanVertexBuffer vertex_buffer;
        VulkanIndexBuffer index_buffer;
        VulkanCommandPool render_command_pool;
        VulkanCommandPool transient_command_pool;
        std::vector<VkCommandBuffer> command_buffers;
        VulkanQueue graphics_queue = nullptr;
        VulkanQueue present_queue = nullptr;
        u32 current_frame_index = 0;

    public:
        VulkanRenderer(const Config& config);

        ~VulkanRenderer();

        void begin_frame();

        void end_frame();

        void render();

    private:
        void allocate_command_buffers();

        void initialize_queues();

        void do_commands(const std::function<void(const VulkanCommandBuffer&)>& on_record_commands) const;

        void begin_command_buffer(const VulkanCommandBuffer& command_buffer) const;

        void end_command_buffer(const VulkanCommandBuffer& command_buffer) const;
    };
}