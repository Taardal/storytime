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
    };
}