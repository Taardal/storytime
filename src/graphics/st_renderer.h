#pragma once

#include "graphics/st_quad.h"
#include "graphics/st_quad_vertex.h"
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
#include "system/st_metrics.h"
#include "window/st_window.h"

namespace Storytime {
    struct RendererConfig {
        std::string name = "";
        Dispatcher* dispatcher = nullptr;
        Window* window = nullptr;
        Metrics* metrics = nullptr;
        VulkanContext* context = nullptr;
        VulkanPhysicalDevice* physical_device = nullptr;
        VulkanDevice* device = nullptr;
        VulkanCommandPool* command_pool = nullptr;
        u32 max_frames_in_flight = 0;
        bool validation_layers_enabled = false;
    };

    class Renderer {
    public:
        typedef RendererConfig Config;

    private:
        static constexpr u32 max_batches_per_frame = 4;

        static constexpr u32 max_quads_per_batch = 10'000;
        static constexpr u32 max_textures_per_batch = 16;

        static constexpr u32 max_vertices_per_quad = 4;
        static constexpr u32 max_indices_per_quad = 6;
        static constexpr u32 max_vertices_per_batch = max_quads_per_batch * max_vertices_per_quad;

        static constexpr u32 frame_descriptor_set_binding = 0;
        static constexpr u32 frame_descriptor_count = 1;

        static constexpr u32 batch_descriptor_set_binding = 1;
        static constexpr u32 batch_descriptor_count = max_textures_per_batch;

        typedef glm::vec2 TextureCoordinate;

        static constexpr std::array<glm::vec2, max_vertices_per_quad> texture_coordinates = {
            glm::vec2(0.0f, 0.0f), // Top left
            glm::vec2(1.0f, 0.0f), // Top right
            glm::vec2(1.0f, 1.0f), // Bottom right
            glm::vec2(0.0f, 1.0f), // Bottom left
        };

        static constexpr std::array<QuadVertex, max_vertices_per_quad> vertices = {
            QuadVertex{{ 0.0f, 0.0f, 0.0f, 1.0f }, glm::vec2(0.0f, 0.0f)},
            QuadVertex{{ 1.0f, 0.0f, 0.0f, 1.0f }, glm::vec2(1.0f, 0.0f)},
            QuadVertex{{ 1.0f, 1.0f, 0.0f, 1.0f }, glm::vec2(1.0f, 1.0f)},
            QuadVertex{{ 0.0f, 1.0f, 0.0f, 1.0f }, glm::vec2(0.0f, 1.0f)},
        };

        typedef u16 Index;

        static constexpr std::array<Index, max_indices_per_quad> indices = {
            0, 1, 2, 2, 3, 0
        };

    private:
        Config config;

        // Permanent
        VulkanSwapchain swapchain;
        VulkanCommandPool initialization_command_pool;
        VulkanCommandPool runtime_command_pool;
        VulkanVertexBuffer vertex_buffer;
        VulkanIndexBuffer index_buffer;
        std::vector<VulkanUniformBuffer> uniform_buffers{};
        VulkanDescriptorPool frame_descriptor_pool;
        VkDescriptorSetLayout frame_descriptor_set_layout;
        VulkanDescriptorPool batch_descriptor_pool;
        VkDescriptorSetLayout batch_descriptor_set_layout;
        VulkanGraphicsPipeline graphics_pipeline;
        VkSampler sampler;

        // Per-frame
        std::vector<VkFence> in_flight_fences;
        std::vector<VkSemaphore> image_available_semaphores;
        std::vector<VkSemaphore> render_finished_semaphores;
        std::vector<VkCommandBuffer> frame_command_buffers{};
        std::vector<VkDescriptorSet> frame_descriptor_sets{};
        std::vector<Frame> frames{};
        u32 frame_index = 0;
        u32 frame_counter = 0;
        f64 frame_delta_ms = 0.0;
        TimePoint frame_delta_time;
        TimePoint frame_start_time;
        TimePoint frame_end_time;

        // Per-batch
        std::vector<std::vector<VulkanInstanceBuffer>> batch_vertex_buffers;
        std::vector<std::vector<VkDescriptorSet>> batch_descriptor_sets{};
        std::vector<std::vector<std::vector<QuadInstanceData>>> quad_batches{};
        std::vector<std::vector<std::vector<std::shared_ptr<VulkanImage>>>> texture_batches{};
        std::shared_ptr<VulkanImage> placeholder_texture = nullptr;

    public:
        Renderer(const Config& config);

        ~Renderer();

        void wait_until_idle() const;

        void set_view_projection(const ViewProjection& view_projection);

        bool begin_frame();

        void end_frame();

        void render_quad(const Quad& quad);

    private:
        Frame& get_current_frame();

        void reset_current_frame();

        void flush_current_batch();

        std::vector<VulkanUniformBuffer> create_uniform_buffers();

        VulkanDescriptorPool create_frame_descriptor_pool();

        VulkanDescriptorPool create_batch_descriptor_pool();

        VkDescriptorSetLayout create_frame_descriptor_set_layout() const;

        void destroy_frame_descriptor_set_layout() const;

        VkDescriptorSetLayout create_batch_descriptor_set_layout() const;

        void destroy_batch_descriptor_set_layout() const;

        VulkanGraphicsPipeline create_graphics_pipeline();

        VkShaderModule create_shader_module(const std::filesystem::path& path) const;

        void destroy_shader_module(VkShaderModule shader_module) const;

        VkSampler create_sampler() const;

        void destroy_sampler() const;

        void create_sync_objects();

        void destroy_sync_objects() const;

        void allocate_frame_command_buffers();

        void allocate_frame_descriptor_sets();

        void write_frame_descriptors() const;

        void prepare_frames();

        void create_batch_vertex_buffers();

        void allocate_batch_descriptor_sets();

        void prepare_quad_batches();

        void prepare_texture_batches();

        void write_batch_descriptors(const VulkanDescriptorSet& descriptor_set, const std::vector<std::shared_ptr<VulkanImage>>& textures) const;

        void begin_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const;

        void end_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const;

        VulkanCommandBuffer begin_one_time_submit_command_buffer() const;

        void end_one_time_submit_command_buffer(const VulkanCommandBuffer& command_buffer) const;

        void record_and_submit_commands(const RecordCommandsFn& record_commands) const;

        void record_and_submit_commands(const RecordAndSubmitCommandsFn& record_and_submit_commands) const;

        void set_object_name(void* object, VkObjectType object_type, const char* object_name) const;
    };
}
