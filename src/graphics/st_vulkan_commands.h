#pragma once

namespace Storytime {
    struct BeginRenderPassCommand {
        VkRenderPassBeginInfo* render_pass_begin_info = nullptr;
        VkSubpassContents subpass_contents = VK_SUBPASS_CONTENTS_MAX_ENUM;
    };

    struct BindPipelineCommand {
        VkPipeline pipeline = nullptr;
        VkPipelineBindPoint bind_point = VK_PIPELINE_BIND_POINT_MAX_ENUM;
    };

    struct SetViewportsCommand {
        VkViewport* viewports = nullptr;
        u32 first_viewport = 0;
        u32 viewport_count = 0;
    };

    struct SetScissorsCommand {
        VkRect2D* scissors = nullptr;
        u32 first_scissor = 0;
        u32 scissor_count = 0;
    };

    struct DrawCommand {
        u32 vertex_count = 0;
        u32 instance_count = 0;
        u32 first_vertex = 0;
        u32 first_instance = 0;
    };

    struct DrawIndexedCommand {
        u32 index_count = 0;
        u32 instance_count = 0;
        u32 first_index = 0;
        i32 vertex_offset = 0;
        u32 first_instance = 0;
    };

    struct BindVertexBuffersCommand {
        u32 first_binding = 0;
        u32 binding_count = 0;
        VkBuffer* vertex_buffers = nullptr;
        VkDeviceSize* offsets = nullptr;
    };

    struct BindIndexBufferCommand {
        VkBuffer index_buffer = nullptr;
        VkDeviceSize offset = 0;
        VkIndexType index_type = VK_INDEX_TYPE_MAX_ENUM;
    };

    struct CopyBufferCommand {
        VkBuffer src_buffer = nullptr;
        VkBuffer dst_buffer = nullptr;
        u32 region_count = 0;
        const VkBufferCopy* regions = nullptr;
    };

    struct BindDescriptorSetsCommand {
        VkPipelineBindPoint pipeline_bind_point = VK_PIPELINE_BIND_POINT_MAX_ENUM;
        VkPipelineLayout pipeline_layout = nullptr;
        u32 first_set = 0;
        u32 descriptor_set_count = 0;
        const VkDescriptorSet* descriptor_sets = nullptr;
        u32 dynamic_offset_count = 0;
        const u32* dynamic_offsets = nullptr;
    };

    struct PipelineBarrierCommand {
        VkPipelineStageFlags src_stage_mask = VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM;
        VkPipelineStageFlags dst_stage_mask = VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM;
        VkDependencyFlags dependency_flags = VK_DEPENDENCY_FLAG_BITS_MAX_ENUM;
        u32 memory_barrier_count = 0;
        const VkMemoryBarrier* memory_barriers = nullptr;
        u32 buffer_memory_barrier_count = 0;
        const VkBufferMemoryBarrier* buffer_memory_barriers = nullptr;
        u32 image_memory_barrier_count = 0;
        const VkImageMemoryBarrier* image_memory_barriers = nullptr;
    };

    struct CopyBufferToImageCommand {
        VkBuffer src_buffer = nullptr;
        VkImage dst_image = nullptr;
        VkImageLayout dst_image_layout = VK_IMAGE_LAYOUT_MAX_ENUM;
        u32 copy_region_count = 0;
        const VkBufferImageCopy* copy_region = nullptr;
    };
}