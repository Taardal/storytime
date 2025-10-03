#pragma once

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

        struct BeginRenderPassCommand {
            VkRenderPassBeginInfo* render_pass_begin_info = nullptr;
            VkSubpassContents subpass_contents = VK_SUBPASS_CONTENTS_MAX_ENUM;
        };

        void begin_render_pass(const BeginRenderPassCommand& command) const;

        void begin_render_pass(const VkRenderPassBeginInfo& render_pass_begin_info, const VkSubpassContents& subpass_contents) const;

        void end_render_pass() const;

        struct BindPipelineCommand {
            VkPipeline pipeline = nullptr;
            VkPipelineBindPoint bind_point = VK_PIPELINE_BIND_POINT_MAX_ENUM;
        };

        void bind_pipeline(const BindPipelineCommand& command) const;

        void bind_pipeline(VkPipelineBindPoint bind_point, VkPipeline pipeline) const;

        struct SetViewportsCommand {
            VkViewport* viewports = nullptr;
            u32 first_viewport = 0;
            u32 viewport_count = 0;
        };

        void set_viewports(const SetViewportsCommand& command) const;

        void set_viewport(const VkViewport& viewport) const;

        void set_viewports(u32 first_viewport, u32 viewport_count, const VkViewport* viewports) const;

        struct SetScissorsCommand {
            VkRect2D* scissors = nullptr;
            u32 first_scissor = 0;
            u32 scissor_count = 0;
        };

        void set_scissors(const SetScissorsCommand& command) const;

        void set_scissor(const VkRect2D& scissor) const;

        void set_scissors(u32 first_scissor, u32 scissor_count, const VkRect2D* scissors) const;

        struct DrawCommand {
            u32 vertex_count = 0;
            u32 instance_count = 0;
            u32 first_vertex = 0;
            u32 first_instance = 0;
        };

        void draw(const DrawCommand& command) const;

        void draw(u32 vertex_count, u32 instance_count, u32 first_vertex, u32 first_instance) const;

        struct DrawIndexedCommand {
            u32 index_count = 0;
            u32 instance_count = 0;
            u32 first_index = 0;
            i32 vertex_offset = 0;
            u32 first_instance = 0;
        };

        void draw_indexed(const DrawIndexedCommand& command) const;

        void draw_indexed(u32 index_count, u32 instance_count, u32 first_index, i32 vertex_offset, u32 first_instance) const;

        struct BindVertexBuffersCommand {
            u32 first_binding = 0;
            u32 binding_count = 0;
            VkBuffer* vertex_buffers = nullptr;
            VkDeviceSize* offsets = nullptr;
        };

        void bind_vertex_buffers(const ::Storytime::VulkanCommandBuffer::BindVertexBuffersCommand& command) const;

        void bind_vertex_buffers(u32 first_binding, u32 binding_count, const VkBuffer* vertex_buffers, const VkDeviceSize* offsets = nullptr) const;

        void bind_vertex_buffer(VkBuffer vertex_buffer, const VkDeviceSize* offsets = nullptr) const;

        struct BindIndexBufferCommand {
            VkBuffer index_buffer = nullptr;
            VkDeviceSize offset = 0;
            VkIndexType index_type = VK_INDEX_TYPE_MAX_ENUM;
        };

        void bind_index_buffer(const BindIndexBufferCommand& command) const;

        void bind_index_buffer(VkBuffer index_buffer, VkDeviceSize offset, VkIndexType index_type) const;

        struct CopyBufferCommand {
            VkBuffer src_buffer = nullptr;
            VkBuffer dst_buffer = nullptr;
            u32 region_count = 0;
            const VkBufferCopy* regions = nullptr;
        };

        void copy_buffer(const CopyBufferCommand& command) const;

        void copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, u32 region_count, const VkBufferCopy* regions) const;

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

        void bind_descriptor_sets(VkPipelineBindPoint pipeline_bind_point, VkPipelineLayout pipeline_layout, u32 first_set, u32 descriptor_set_count, const VkDescriptorSet* descriptor_sets, u32 dynamic_offset_count, const u32* dynamic_offsets) const;

        void bind_descriptor_set(VkPipelineBindPoint pipeline_bind_point, VkPipelineLayout pipeline_layout, VkDescriptorSet descriptor_set, u32 dynamic_offset_count, const u32* dynamic_offsets) const;
    };
}