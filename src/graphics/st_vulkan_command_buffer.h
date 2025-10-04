#pragma once

#include "graphics/st_vulkan_commands.h"

namespace Storytime {
    class VulkanCommandBuffer {
        VkCommandBuffer command_buffer = nullptr;

    public:
        VulkanCommandBuffer() = default;

        VulkanCommandBuffer(VkCommandBuffer command_buffer);

        operator VkCommandBuffer() const;

        VkResult begin(VkCommandBufferUsageFlags usage_flags = 0) const;

        VkResult begin(const VkCommandBufferBeginInfo& command_buffer_begin_info) const;

        VkResult end() const;

        VkResult reset(VkCommandBufferResetFlags reset_flags = 0) const;

        void begin_render_pass(const BeginRenderPassCommand& command) const;

        void begin_render_pass(const VkRenderPassBeginInfo& render_pass_begin_info, const VkSubpassContents& subpass_contents) const;

        void end_render_pass() const;

        void bind_pipeline(const BindPipelineCommand& command) const;

        void bind_pipeline(VkPipelineBindPoint bind_point, VkPipeline pipeline) const;

        void set_viewports(const SetViewportsCommand& command) const;

        void set_viewport(const VkViewport& viewport) const;

        void set_viewports(u32 first_viewport, u32 viewport_count, const VkViewport* viewports) const;

        void set_scissors(const SetScissorsCommand& command) const;

        void set_scissor(const VkRect2D& scissor) const;

        void set_scissors(u32 first_scissor, u32 scissor_count, const VkRect2D* scissors) const;

        void draw(const DrawCommand& command) const;

        void draw(u32 vertex_count, u32 instance_count, u32 first_vertex, u32 first_instance) const;

        void draw_indexed(const DrawIndexedCommand& command) const;

        void draw_indexed(u32 index_count, u32 instance_count, u32 first_index, i32 vertex_offset, u32 first_instance) const;

        void bind_vertex_buffers(const BindVertexBuffersCommand& command) const;

        void bind_vertex_buffers(u32 first_binding, u32 binding_count, const VkBuffer* vertex_buffers, const VkDeviceSize* offsets = nullptr) const;

        void bind_vertex_buffer(VkBuffer vertex_buffer, const VkDeviceSize* offsets = nullptr) const;

        void bind_index_buffer(const BindIndexBufferCommand& command) const;

        void bind_index_buffer(VkBuffer index_buffer, VkDeviceSize offset, VkIndexType index_type) const;

        void copy_buffer(const CopyBufferCommand& command) const;

        void copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, u32 region_count, const VkBufferCopy* regions) const;

        void bind_descriptor_sets(const BindDescriptorSetsCommand& command) const;

        void bind_descriptor_sets(VkPipelineBindPoint pipeline_bind_point, VkPipelineLayout pipeline_layout, u32 first_set, u32 descriptor_set_count, const VkDescriptorSet* descriptor_sets, u32 dynamic_offset_count, const u32* dynamic_offsets) const;

        void bind_descriptor_set(VkPipelineBindPoint pipeline_bind_point, VkPipelineLayout pipeline_layout, VkDescriptorSet descriptor_set, u32 dynamic_offset_count, const u32* dynamic_offsets) const;

        void pipeline_barrier(const PipelineBarrierCommand& command) const;

        void pipeline_barrier(
            VkPipelineStageFlags src_stage_mask,
            VkPipelineStageFlags dst_stage_mask,
            VkDependencyFlags dependency_flags,
            u32 memory_barrier_count,
            const VkMemoryBarrier* memory_barriers,
            u32 buffer_memory_barrier_count,
            const VkBufferMemoryBarrier* buffer_memory_barriers,
            u32 image_memory_barrier_count,
            const VkImageMemoryBarrier* image_memory_barriers
        ) const;

        void copy_buffer_to_image(const CopyBufferToImageCommand& command) const;

        void copy_buffer_to_image(
            VkBuffer src_buffer,
            VkImage dst_image,
            VkImageLayout dst_image_layout,
            u32 copy_region_count,
            const VkBufferImageCopy* copy_region
        ) const;
    };
}