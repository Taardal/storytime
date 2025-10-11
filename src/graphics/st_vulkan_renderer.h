#pragma once

#include "graphics/st_vulkan_command_pool.h"
#include "graphics/st_vulkan_context.h"
#include "graphics/st_vulkan_descriptor_pool.h"
#include "graphics/st_vulkan_device.h"
#include "graphics/st_vulkan_frame.h"
#include "graphics/st_vulkan_graphics_pipeline.h"
#include "graphics/st_vulkan_image.h"
#include "graphics/st_vulkan_index_buffer.h"
#include "graphics/st_vulkan_physical_device.h"
#include "graphics/st_vulkan_swapchain.h"
#include "graphics/st_vulkan_uniform_buffer.h"
#include "graphics/st_vulkan_vertex_buffer.h"
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
        VulkanContext context;
        VulkanPhysicalDevice physical_device;
        VulkanDevice device;
        VulkanSwapchain swapchain;
        VulkanCommandPool initialization_command_pool;
        VulkanCommandPool runtime_command_pool;
        VulkanVertexBuffer vertex_buffer;
        VulkanIndexBuffer index_buffer;
        std::vector<VulkanUniformBuffer> uniform_buffers{};
        VulkanDescriptorPool descriptor_pool;
        VkDescriptorSetLayout descriptor_set_layout;
        VulkanGraphicsPipeline graphics_pipeline;
        std::vector<VkFence> in_flight_fences;
        std::vector<VkSemaphore> image_available_semaphores;
        std::vector<VkSemaphore> render_finished_semaphores;
        std::vector<VkCommandBuffer> command_buffers{};
        std::vector<VkDescriptorSet> descriptor_sets{};
        std::vector<Frame> frames{};
        VkSampler sampler;
        VulkanImage texture;
        u32 current_frame_index = 0;

    public:
        VulkanRenderer(const Config& config);

        ~VulkanRenderer();

        void begin_frame();

        void end_frame();

        void render() const;

    private:
        std::vector<VulkanUniformBuffer> create_uniform_buffers();

        VulkanDescriptorPool create_descriptor_pool();

        VkDescriptorSetLayout create_descriptor_set_layout() const;

        void destroy_descriptor_set_layout() const;

        VulkanGraphicsPipeline create_graphics_pipeline();

        VkShaderModule create_shader_module(const std::filesystem::path& path) const;

        void destroy_shader_module(VkShaderModule shader_module) const;

        VkSampler create_sampler() const;

        void destroy_sampler() const;

        void create_sync_objects();

        void destroy_sync_objects() const;

        void allocate_command_buffers();

        void allocate_descriptor_sets();

        void write_descriptors() const;

        void prepare_frames();

        void begin_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const;

        void end_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const;

        VulkanCommandBuffer begin_one_time_submit_command_buffer() const;

        void end_one_time_submit_command_buffer(const VulkanCommandBuffer& command_buffer) const;

        void record_and_submit_commands(const RecordCommandsFn& record_commands) const;

        void record_and_submit_commands(const RecordAndSubmitCommandsFn& record_and_submit_commands) const;
    };
}
