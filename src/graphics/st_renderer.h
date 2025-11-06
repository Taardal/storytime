#pragma once

#include "graphics/st_quad.h"
#include "graphics/st_quad_vertex.h"
#include "graphics/st_view_projection.h"
#include "graphics/st_vulkan_command_pool.h"
#include "graphics/st_vulkan_context.h"
#include "graphics/st_vulkan_descriptor_pool.h"
#include "graphics/st_vulkan_device.h"
#include "graphics/st_frame.h"
#include "graphics/st_vulkan_graphics_pipeline.h"
#include "graphics/st_vulkan_image.h"
#include "graphics/st_vulkan_index_buffer.h"
#include "graphics/st_vulkan_swapchain.h"
#include "graphics/st_vulkan_vertex_buffer.h"
#include "system/st_dispatcher.h"
#include "system/st_file_reader.h"
#include "system/st_metrics.h"
#include "window/st_window.h"

namespace Storytime {
    struct RendererConfig {
        std::string name = "Renderer";
        Dispatcher* dispatcher = nullptr;
        Window* window = nullptr;
        FileReader* file_reader = nullptr;
        Metrics* metrics = nullptr;
        VulkanContext* context = nullptr;
        VulkanDevice* device = nullptr;
        VulkanSwapchain* swapchain = nullptr;
        u32 frame_count = 0;

        const RendererConfig& assert_valid() const {
            ST_ASSERT_NOT_NULL(dispatcher);
            ST_ASSERT_NOT_NULL(window);
            ST_ASSERT_NOT_NULL(file_reader);
            ST_ASSERT_NOT_NULL(metrics);
            ST_ASSERT_NOT_NULL(context);
            ST_ASSERT_NOT_NULL(device);
            ST_ASSERT_NOT_NULL(swapchain);
            ST_ASSERT_GREATER_THAN_ZERO(frame_count);
            return *this;
        }
    };

    class Renderer {
    public:
        typedef RendererConfig Config;

    private:
        // Vertices
        static constexpr u32 max_vertices_per_quad = 4;
        static constexpr std::array<QuadVertex, max_vertices_per_quad> base_quad_vertices = {
            QuadVertex{ .position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), .texture_coordinate = glm::vec2(0.0f, 0.0f) }, // Top left
            QuadVertex{ .position = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), .texture_coordinate = glm::vec2(1.0f, 0.0f) }, // Top right
            QuadVertex{ .position = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), .texture_coordinate = glm::vec2(1.0f, 1.0f) }, // Bottom right
            QuadVertex{ .position = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), .texture_coordinate = glm::vec2(0.0f, 1.0f) }, // Bottom left
        };

        // Indices
        typedef u16 Index;
        static constexpr VkIndexType index_type = VK_INDEX_TYPE_UINT16;
        static constexpr u32 max_indices_per_quad = 6;
        static constexpr std::array<Index, max_indices_per_quad> base_quad_indices = {
            0, 1, 2, 2, 3, 0
        };

        // Batching
        static constexpr u32 max_batches_per_frame = 8;
        static constexpr u32 max_textures_per_batch = 16;
        static constexpr u32 max_quads_per_batch = 20'000;
        static constexpr u32 max_vertices_per_batch = max_quads_per_batch * max_vertices_per_quad;

    private:
        Config config;
        VulkanCommandPool init_command_pool;
        VulkanCommandPool frame_command_pool;
        VulkanDescriptorPool frame_descriptor_pool;
        VkDescriptorSetLayout frame_descriptor_set_layout;
        VulkanDescriptorPool batch_descriptor_pool;
        VkDescriptorSetLayout batch_descriptor_set_layout;
        VulkanGraphicsPipeline quad_graphics_pipeline;
        VulkanVertexBuffer base_quad_vertex_buffer;
        VulkanIndexBuffer base_quad_index_buffer;
        VkSampler texture_sampler;
        Shared<VulkanImage> placeholder_texture;
        std::vector<Frame> frames;
        u32 frame_index = 0;

    public:
        Renderer(const Config& config);

        ~Renderer();

        VkCommandBuffer get_frame_command_buffer() const;

        void wait_until_idle() const;

        const Frame* begin_frame() const;

        void end_frame();

        void begin_render() const;

        void end_render();

        void set_view_projection(const ViewProjection& view_projection) const;

        void render_quad(const Quad& quad);

    private:
        void flush(Frame& frame, const Batch& batch) const;

        void reset(Frame& frame) const;

        void begin_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const;

        void end_frame_command_buffer(const VulkanCommandBuffer& command_buffer) const;

        void write_frame_descriptors(const Frame& frame) const;

        void write_batch_descriptors(const Batch& batch) const;

        VulkanCommandPool create_init_command_pool();

        VulkanCommandPool create_frame_command_pool();

        VulkanDescriptorPool create_frame_descriptor_pool();

        VulkanDescriptorPool create_batch_descriptor_pool();

        VkDescriptorSetLayout create_frame_descriptor_set_layout() const;

        void destroy_frame_descriptor_set_layout() const;

        VkDescriptorSetLayout create_batch_descriptor_set_layout() const;

        void destroy_batch_descriptor_set_layout() const;

        VulkanGraphicsPipeline create_quad_graphics_pipeline();

        VkShaderModule create_shader_module(const std::filesystem::path& path, std::string_view name = "") const;

        void destroy_shader_module(VkShaderModule shader_module) const;

        VulkanVertexBuffer create_base_quad_vertex_buffer();

        VulkanIndexBuffer create_base_quad_index_buffer();

        VkSampler create_texture_sampler() const;

        void destroy_texture_sampler() const;

        Shared<VulkanImage> create_placeholder_texture() const;

        void destroy_placeholder_texture();

        std::vector<Frame> create_frames();

        void destroy_frames();
    };
}
