#include "st_vulkan_command_buffer.h"

namespace Storytime {
    VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer command_buffer) : command_buffer(command_buffer) {}

    VulkanCommandBuffer::operator VkCommandBuffer() const {
        return command_buffer;
    }

    VkResult VulkanCommandBuffer::begin(VkCommandBufferUsageFlags usage_flags) const {
        VkCommandBufferBeginInfo command_buffer_begin_info{};
        command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.flags = usage_flags;
        command_buffer_begin_info.pInheritanceInfo = nullptr;
        return vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
    }

    VkResult VulkanCommandBuffer::begin(const VkCommandBufferBeginInfo& command_buffer_begin_info) const {
        return vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
    }

    VkResult VulkanCommandBuffer::end() const {
        return vkEndCommandBuffer(command_buffer);
    }

    VkResult VulkanCommandBuffer::reset(VkCommandBufferResetFlags reset_flags) const {
        return vkResetCommandBuffer(command_buffer, reset_flags);
    }

    void VulkanCommandBuffer::begin_render_pass(const BeginRenderPassCommand& command) const {
        vkCmdBeginRenderPass(command_buffer, command.render_pass_begin_info, command.subpass_contents);
    }

    void VulkanCommandBuffer::begin_render_pass(const VkRenderPassBeginInfo& render_pass_begin_info, const VkSubpassContents& subpass_contents) const {
        vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, subpass_contents);
    }

    void VulkanCommandBuffer::end_render_pass() const {
        vkCmdEndRenderPass(command_buffer);
    }

    void VulkanCommandBuffer::bind_pipeline(const BindPipelineCommand& command) const {
        vkCmdBindPipeline(command_buffer, command.bind_point, command.pipeline);
    }

    void VulkanCommandBuffer::bind_pipeline(VkPipelineBindPoint bind_point, VkPipeline pipeline) const {
        vkCmdBindPipeline(command_buffer, bind_point, pipeline);
    }

    void VulkanCommandBuffer::set_viewport(const SetViewportCommand& command) const {
        vkCmdSetViewport(command_buffer, command.first_viewport, command.viewport_count, command.viewports);
    }

    void VulkanCommandBuffer::set_viewport(u32 first_viewport, u32 viewport_count, const VkViewport* viewports) const {
        vkCmdSetViewport(command_buffer, first_viewport, viewport_count, viewports);
    }

    void VulkanCommandBuffer::set_scissor(const SetScissorCommand& command) const {
        vkCmdSetScissor(command_buffer, command.first_scissor, command.scissor_count, command.scissors);
    }

    void VulkanCommandBuffer::set_scissor(u32 first_scissor, u32 scissor_count, const VkRect2D* scissors) const {
        vkCmdSetScissor(command_buffer, first_scissor, scissor_count, scissors);
    }

    void VulkanCommandBuffer::draw(const DrawCommand& command) const {
        vkCmdDraw(command_buffer, command.vertex_count, command.instance_count, command.first_vertex, command.first_instance);
    }

    void VulkanCommandBuffer::draw(u32 vertex_count, u32 instance_count, u32 first_vertex, u32 first_instance) const {
        vkCmdDraw(command_buffer, vertex_count, instance_count, first_vertex, first_instance);
    }
}
