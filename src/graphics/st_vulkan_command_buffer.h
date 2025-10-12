#pragma once

#include "graphics/st_vulkan_command_callbacks.h"
#include "graphics/st_vulkan_device.h"
#include "graphics/st_vulkan_queue.h"

namespace Storytime {
    using WithCommandsFn = std::function<void(VkCommandBuffer)>;

    class VulkanCommandBuffer {
        VkCommandBuffer command_buffer = nullptr;

    public:
        VulkanCommandBuffer(VkCommandBuffer command_buffer);

        operator VkCommandBuffer() const;

        VkResult begin(VkCommandBufferUsageFlags usage_flags = 0) const;

        VkResult begin(const VkCommandBufferBeginInfo& begin_info) const;

        VkResult end() const;

        VkResult reset(VkCommandBufferResetFlags reset_flags = 0) const;

        void begin_one_time_submit() const;

        void end_one_time_submit(
            const VulkanQueue& queue,
            const VulkanDevice& device
        ) const;

        void record_and_submit(
            const VulkanQueue& queue,
            const VulkanDevice& device,
            const RecordCommandsFn& record_commands
        ) const;

        void record_and_submit(
            const VulkanQueue& queue,
            const VulkanDevice& device,
            const RecordAndSubmitCommandsFn& record_and_submit_commands
        ) const;

        void with_commands(const WithCommandsFn& with_commands) const;

        struct BeginRenderPassCommand {
            VkRenderPassBeginInfo* render_pass_begin_info = nullptr;
            VkSubpassContents subpass_contents = VK_SUBPASS_CONTENTS_MAX_ENUM;
        };

        void begin_render_pass(const BeginRenderPassCommand& command) const;

        void begin_render_pass(
            const VkRenderPassBeginInfo& render_pass_begin_info,
            const VkSubpassContents& subpass_contents
        ) const;

        void end_render_pass() const;

        struct BindPipelineCommand {
            VkPipeline pipeline = nullptr;
            VkPipelineBindPoint bind_point = VK_PIPELINE_BIND_POINT_MAX_ENUM;
        };

        void bind_pipeline(const BindPipelineCommand& command) const;

        void bind_pipeline(
            VkPipelineBindPoint bind_point,
            VkPipeline pipeline
        ) const;

        struct SetViewportsCommand {
            VkViewport* viewports = nullptr;
            u32 first_viewport = 0;
            u32 viewport_count = 0;
        };

        void set_viewports(const SetViewportsCommand& command) const;

        void set_viewports(
            u32 first_viewport,
            u32 viewport_count,
            const VkViewport* viewports
        ) const;

        void set_viewport(const VkViewport& viewport) const;

        struct SetScissorsCommand {
            VkRect2D* scissors = nullptr;
            u32 first_scissor = 0;
            u32 scissor_count = 0;
        };

        void set_scissors(const SetScissorsCommand& command) const;

        void set_scissors(
            u32 first_scissor,
            u32 scissor_count,
            const VkRect2D* scissors
        ) const;

        void set_scissor(const VkRect2D& scissor) const;

        struct DrawCommand {
            u32 vertex_count = 0;
            u32 instance_count = 0;
            u32 first_vertex = 0;
            u32 first_instance = 0;
        };

        void draw(const DrawCommand& command) const;

        void draw(
            u32 vertex_count,
            u32 instance_count,
            u32 first_vertex,
            u32 first_instance
        ) const;

        struct DrawIndexedCommand {
            u32 index_count = 0;
            u32 instance_count = 0;
            u32 first_index = 0;
            i32 vertex_offset = 0;
            u32 first_instance = 0;
        };

        void draw_indexed(const DrawIndexedCommand& command) const;

        void draw_indexed(
            u32 index_count,
            u32 instance_count,
            u32 first_index,
            i32 vertex_offset,
            u32 first_instance
        ) const;

        struct BindVertexBuffersCommand {
            u32 first_binding = 0;
            u32 binding_count = 0;
            VkBuffer* vertex_buffers = nullptr;
            VkDeviceSize* offsets = nullptr;
        };

        void bind_vertex_buffers(const BindVertexBuffersCommand& command) const;

        void bind_vertex_buffers(
            u32 first_binding,
            u32 binding_count,
            const VkBuffer* vertex_buffers,
            const VkDeviceSize* offsets = nullptr
        ) const;

        void bind_vertex_buffer(
            VkBuffer vertex_buffer,
            const VkDeviceSize* offsets = nullptr
        ) const;

        struct BindIndexBufferCommand {
            VkBuffer index_buffer = nullptr;
            VkDeviceSize offset = 0;
            VkIndexType index_type = VK_INDEX_TYPE_MAX_ENUM;
        };

        void bind_index_buffer(const BindIndexBufferCommand& command) const;

        void bind_index_buffer(
            VkBuffer index_buffer,
            VkDeviceSize offset,
            VkIndexType index_type
        ) const;

        struct CopyBufferCommand {
            VkBuffer src_buffer = nullptr;
            VkBuffer dst_buffer = nullptr;
            u32 region_count = 0;
            const VkBufferCopy* regions = nullptr;
        };

        void copy_buffer(const CopyBufferCommand& command) const;

        void copy_buffer(
            VkBuffer src_buffer,
            VkBuffer dst_buffer,
            u32 region_count,
            const VkBufferCopy* regions
        ) const;

        struct BindDescriptorSetsCommand {
            VkPipelineBindPoint pipeline_bind_point = VK_PIPELINE_BIND_POINT_MAX_ENUM;
            VkPipelineLayout pipeline_layout = nullptr;
            u32 first_set = 0;
            u32 descriptor_set_count = 0;
            const VkDescriptorSet* descriptor_sets = nullptr;
            u32 dynamic_offset_count = 0;
            const u32* dynamic_offsets = nullptr;
        };

        void bind_descriptor_sets(const BindDescriptorSetsCommand& command) const;

        void bind_descriptor_sets(
            VkPipelineBindPoint pipeline_bind_point,
            VkPipelineLayout pipeline_layout,
            u32 first_set,
            u32 descriptor_set_count,
            const VkDescriptorSet* descriptor_sets,
            u32 dynamic_offset_count,
            const u32* dynamic_offsets
        ) const;

        void bind_descriptor_set(
            VkPipelineBindPoint pipeline_bind_point,
            VkPipelineLayout pipeline_layout,
            VkDescriptorSet descriptor_set,
            u32 dynamic_offset_count,
            const u32* dynamic_offsets
        ) const;

        struct PipelineBarrierCommand {
            VkPipelineStageFlags src_stage_mask = 0;
            VkPipelineStageFlags dst_stage_mask = 0;
            VkDependencyFlags dependency_flags = 0;
            u32 memory_barrier_count = 0;
            const VkMemoryBarrier* memory_barriers = nullptr;
            u32 buffer_memory_barrier_count = 0;
            const VkBufferMemoryBarrier* buffer_memory_barriers = nullptr;
            u32 image_memory_barrier_count = 0;
            const VkImageMemoryBarrier* image_memory_barriers = nullptr;
        };

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

        struct CopyBufferToImageCommand {
            VkBuffer src_buffer = nullptr;
            VkImage dst_image = nullptr;
            VkImageLayout dst_image_layout = VK_IMAGE_LAYOUT_MAX_ENUM;
            u32 copy_region_count = 0;
            const VkBufferImageCopy* copy_region = nullptr;
        };

        void copy_buffer_to_image(const CopyBufferToImageCommand& command) const;

        void copy_buffer_to_image(
            VkBuffer src_buffer,
            VkImage dst_image,
            VkImageLayout dst_image_layout,
            u32 copy_region_count,
            const VkBufferImageCopy* copy_region
        ) const;

        struct BlitImageCommand {
            VkImage src_image;
            VkImageLayout src_image_layout;
            VkImage dst_image;
            VkImageLayout dst_image_layout;
            u32 image_blit_count;
            const VkImageBlit* image_blits;
            VkFilter filter;
        };

        void blit_image(const BlitImageCommand& command) const;

        void blit_image(
            VkImage src_image,
            VkImageLayout src_image_layout,
            VkImage dst_image,
            VkImageLayout dst_image_layout,
            u32 image_blit_count,
            const VkImageBlit* image_blits,
            VkFilter filter
        ) const;
    };
}
