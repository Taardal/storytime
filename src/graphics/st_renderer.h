#pragma once

#include "graphics/st_quad_vertex.h"
#include "graphics/st_texture.h"
#include "graphics/st_texture_coordinate.h"
#include "graphics/st_view_projection.h"
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
#include "system/st_clock.h"
#include "system/st_dispatcher.h"
#include "system/st_log.h"
#include "system/st_metrics.h"
#include "window/st_window.h"

namespace Storytime {
    struct Quad {
        Shared<Texture> texture = nullptr;
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec2 size = {0.0f, 0.0f};
        glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}; 
        f32 rotation_in_degrees = 0.0f;             
        f32 tiling_factor = 1.0f;
    };

    struct RendererConfig {
        Dispatcher* dispatcher = nullptr;
        Metrics* metrics = nullptr;
        Window* window = nullptr;
        std::string name = "";
        u32 max_frames_in_flight = 0;
        bool validation_layers_enabled = false;
    };

    class Renderer {
    public:
        typedef RendererConfig Config;

    private:
        Config config;
        VulkanContext context;
        VulkanPhysicalDevice physical_device;
        VulkanDevice device;
        VulkanSwapchain swapchain;
        VulkanCommandPool initialization_command_pool;
        VulkanCommandPool runtime_command_pool;
        VulkanVertexBuffer vertex_buffer;
        VulkanVertexBuffer instance_buffer;
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
        std::shared_ptr<VulkanImage> texture = nullptr;
        u32 current_frame_index = 0;
        u32 previous_frame_index = 0;
        std::vector<InstanceData> batch{};
        u32 batch_index = 0;
        std::vector<std::shared_ptr<VulkanImage>> batch_textures{};
        u32 texture_index = 0;

        u32 frame_counter = 0;
        f64 frame_delta = 0.0;
        TimePoint frame_delta_time;
        TimePoint frame_start_time;
        TimePoint frame_end_time;

    public:
        Renderer(const Config& config);

        ~Renderer();

        void begin_frame();

        void end_frame();

        void render() const;

        void render_quad(const Quad& quad);

        void render_quad(const Quad& quad, const std::array<TextureCoordinate, 4>& texture_coordinates) { ST_LOG_W("render_quad"); }

        void set_view_projection1(ViewProjection& view_projection);

        void set_view_projection(const ViewProjection& view_projection) const { ST_LOG_W("viewproj"); };

    private:
        void render_batch();

        void reset_metrics();

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
