#include "st_vulkan_command_buffer.h"

#include "graphics/st_vulkan_device.h"

namespace Storytime {
    VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer command_buffer) : command_buffer(command_buffer) {}

    VulkanCommandBuffer::operator VkCommandBuffer() const {
        return command_buffer;
    }

    void VulkanCommandBuffer::with_commands(const WithCommandsFn& with_commands) const {
        if (begin() != VK_SUCCESS) {
            ST_THROW("Could not begin command buffer");
        }
        with_commands(command_buffer);
        if (end() != VK_SUCCESS) {
            ST_THROW("Could not end command buffer");
        }
    }

    void VulkanCommandBuffer::begin_one_time_submit() const {
        if (begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) != VK_SUCCESS) {
            ST_THROW("Could not begin command buffer");
        }
    }

    void VulkanCommandBuffer::end_one_time_submit(const VulkanQueue& queue, const VulkanDevice& device) const {
        if (end() != VK_SUCCESS) {
            ST_THROW("Could not end command buffer");
        }
        if (queue.submit(command_buffer) != VK_SUCCESS) {
            ST_THROW("Could not submit command buffer to queue");
        }
        if (device.wait_until_queue_idle(queue) != VK_SUCCESS) {
            ST_THROW("Could not wait for queue to become idle");
        }
    }

    void VulkanCommandBuffer::record_and_submit(const VulkanQueue& queue, const VulkanDevice& device, const RecordCommandsFn& record_commands) const {
        begin_one_time_submit();
        record_commands(command_buffer);
        end_one_time_submit(queue, device);
    }

    void VulkanCommandBuffer::record_and_submit(const VulkanQueue& queue, const VulkanDevice& device, const RecordAndSubmitCommandsFn& record_and_submit_commands) const {
        record_and_submit_commands([&](const RecordCommandsFn& record_commands) {
            begin_one_time_submit();
            record_commands(command_buffer);
            end_one_time_submit(queue, device);
        });
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

    void VulkanCommandBuffer::set_viewports(const SetViewportsCommand& command) const {
        vkCmdSetViewport(command_buffer, command.first_viewport, command.viewport_count, command.viewports);
    }

    void VulkanCommandBuffer::set_viewports(u32 first_viewport, u32 viewport_count, const VkViewport* viewports) const {
        vkCmdSetViewport(command_buffer, first_viewport, viewport_count, viewports);
    }

    void VulkanCommandBuffer::set_viewport(const VkViewport& viewport) const {
        u32 first_viewport = 0;
        u32 viewport_count = 1;
        vkCmdSetViewport(command_buffer, first_viewport, viewport_count, &viewport);
    }

    void VulkanCommandBuffer::set_scissors(const SetScissorsCommand& command) const {
        vkCmdSetScissor(command_buffer, command.first_scissor, command.scissor_count, command.scissors);
    }

    void VulkanCommandBuffer::set_scissors(u32 first_scissor, u32 scissor_count, const VkRect2D* scissors) const {
        vkCmdSetScissor(command_buffer, first_scissor, scissor_count, scissors);
    }

    void VulkanCommandBuffer::set_scissor(const VkRect2D& scissor) const {
        u32 first_scissor = 0;
        u32 scissor_count = 1;
        vkCmdSetScissor(command_buffer, first_scissor, scissor_count, &scissor);
    }

    void VulkanCommandBuffer::draw(const DrawCommand& command) const {
        vkCmdDraw(command_buffer, command.vertex_count, command.instance_count, command.first_vertex, command.first_instance);
    }

    void VulkanCommandBuffer::draw(u32 vertex_count, u32 instance_count, u32 first_vertex, u32 first_instance) const {
        vkCmdDraw(command_buffer, vertex_count, instance_count, first_vertex, first_instance);
    }

    void VulkanCommandBuffer::draw_indexed(const DrawIndexedCommand& command) const {
        vkCmdDrawIndexed(command_buffer, command.index_count, command.instance_count, command.first_index, command.vertex_offset, command.first_instance);
    }

    void VulkanCommandBuffer::draw_indexed(u32 index_count, u32 instance_count, u32 first_index, i32 vertex_offset, u32 first_instance) const {
        vkCmdDrawIndexed(command_buffer, index_count, instance_count, first_index, vertex_offset, first_instance);
    }

    void VulkanCommandBuffer::bind_vertex_buffers(const BindVertexBuffersCommand& command) const {
        vkCmdBindVertexBuffers(command_buffer, command.first_binding, command.binding_count, command.vertex_buffers, command.offsets);
    }

    void VulkanCommandBuffer::bind_vertex_buffers(u32 first_binding, u32 binding_count, const VkBuffer* vertex_buffers, const VkDeviceSize* offsets) const {
        vkCmdBindVertexBuffers(command_buffer, first_binding, binding_count, vertex_buffers, offsets);
    }

    void VulkanCommandBuffer::bind_vertex_buffer(VkBuffer vertex_buffer, const VkDeviceSize* offsets) const {
        u32 first_binding = 0;
        u32 binding_count = 1;
        vkCmdBindVertexBuffers(command_buffer, first_binding, binding_count, &vertex_buffer, offsets);
    }

    void VulkanCommandBuffer::bind_index_buffer(const BindIndexBufferCommand& command) const {
        vkCmdBindIndexBuffer(command_buffer, command.index_buffer, command.offset, command.index_type);
    }

    void VulkanCommandBuffer::bind_index_buffer(VkBuffer index_buffer, VkDeviceSize offset, VkIndexType index_type) const {
        vkCmdBindIndexBuffer(command_buffer, index_buffer, offset, index_type);
    }

    void VulkanCommandBuffer::copy_buffer(const CopyBufferCommand& command) const {
        vkCmdCopyBuffer(command_buffer, command.src_buffer, command.dst_buffer, command.region_count, command.regions);
    }

    void VulkanCommandBuffer::copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, u32 region_count, const VkBufferCopy* regions) const {
        vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, region_count, regions);
    }

    void VulkanCommandBuffer::bind_descriptor_sets(const BindDescriptorSetsCommand& command) const {
        vkCmdBindDescriptorSets(command_buffer, command.pipeline_bind_point, command.pipeline_layout, command.first_set, command.descriptor_set_count, command.descriptor_sets, command.dynamic_offset_count, command.dynamic_offsets);
    }

    void VulkanCommandBuffer::bind_descriptor_sets(VkPipelineBindPoint pipeline_bind_point, VkPipelineLayout pipeline_layout, u32 first_set, u32 descriptor_set_count, const VkDescriptorSet* descriptor_sets, u32 dynamic_offset_count, const u32* dynamic_offsets) const {
        vkCmdBindDescriptorSets(command_buffer, pipeline_bind_point, pipeline_layout, first_set, descriptor_set_count, descriptor_sets, dynamic_offset_count, dynamic_offsets);
    }

    void VulkanCommandBuffer::bind_descriptor_set(VkPipelineBindPoint pipeline_bind_point, VkPipelineLayout pipeline_layout, VkDescriptorSet descriptor_set, u32 dynamic_offset_count, const u32* dynamic_offsets) const {
        u32 first_set = 0;
        u32 descriptor_set_count = 1;
        vkCmdBindDescriptorSets(command_buffer, pipeline_bind_point, pipeline_layout, first_set, descriptor_set_count, &descriptor_set, dynamic_offset_count, dynamic_offsets);
    }

    void VulkanCommandBuffer::pipeline_barrier(const PipelineBarrierCommand& command) const {
        vkCmdPipelineBarrier(
            command_buffer,
            command.src_stage_mask,
            command.dst_stage_mask,
            command.dependency_flags,
            command.memory_barrier_count,
            command.memory_barriers,
            command.buffer_memory_barrier_count,
            command.buffer_memory_barriers,
            command.image_memory_barrier_count,
            command.image_memory_barriers
        );
    }

    void VulkanCommandBuffer::pipeline_barrier(
        VkPipelineStageFlags src_stage_mask,
        VkPipelineStageFlags dst_stage_mask,
        VkDependencyFlags dependency_flags,
        u32 memory_barrier_count,
        const VkMemoryBarrier* memory_barriers,
        u32 buffer_memory_barrier_count,
        const VkBufferMemoryBarrier* buffer_memory_barriers,
        u32 image_memory_barrier_count,
        const VkImageMemoryBarrier* image_memory_barriers
    ) const {
        vkCmdPipelineBarrier(
            command_buffer,
            src_stage_mask,
            dst_stage_mask,
            dependency_flags,
            memory_barrier_count,
            memory_barriers,
            buffer_memory_barrier_count,
            buffer_memory_barriers,
            image_memory_barrier_count,
            image_memory_barriers
        );
    }

    void VulkanCommandBuffer::copy_buffer_to_image(const CopyBufferToImageCommand& command) const {
        vkCmdCopyBufferToImage(
            command_buffer,
            command.src_buffer,
            command.dst_image,
            command.dst_image_layout,
            command.copy_region_count,
            command.copy_region
        );
    }

    void VulkanCommandBuffer::copy_buffer_to_image(VkBuffer src_buffer, VkImage dst_image, VkImageLayout dst_image_layout, u32 copy_region_count, const VkBufferImageCopy* copy_region) const {
        vkCmdCopyBufferToImage(
            command_buffer,
            src_buffer,
            dst_image,
            dst_image_layout,
            copy_region_count,
            copy_region
        );
    }
}
