#include "st_vulkan_queue.h"

namespace Storytime {
    VulkanQueue::VulkanQueue(VkQueue queue) : queue(queue) {}

    VulkanQueue::operator VkQueue() const {
        return queue;
    }

    VkResult VulkanQueue::submit(u32 submit_count, const VkSubmitInfo& submit_info, VkFence fence) const {
        ST_ASSERT_GREATER_THAN_ZERO(submit_count);
        VkResult result = vkQueueSubmit(queue, submit_count, &submit_info, fence);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not submit to queue: {}", format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanQueue::submit(const SubmitConfig& config) const {
        return submit(config.submit_count, config.submit_info, config.fence);
    }

    VkResult VulkanQueue::submit(const VkSubmitInfo& submit_info, VkFence fence) const {
        return submit(1, submit_info, fence);
    }

    VkResult VulkanQueue::submit(u32 command_buffer_count, const VkCommandBuffer* command_buffers, VkFence fence) const {
        ST_ASSERT_GREATER_THAN_ZERO(command_buffer_count);
        ST_ASSERT_NOT_NULL(command_buffers);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = command_buffer_count;
        submit_info.pCommandBuffers = command_buffers;

        return submit(submit_info, fence);
    }

    VkResult VulkanQueue::submit(const std::vector<VkCommandBuffer>& command_buffers, VkFence fence) const {
        return submit(command_buffers.size(), command_buffers.data(), fence);
    }

    VkResult VulkanQueue::submit(VkCommandBuffer command_buffer, VkFence fence) const {
        return submit(1, &command_buffer, fence);
    }

    VkResult VulkanQueue::present(const VkPresentInfoKHR& present_info) const {
        VkResult result = vkQueuePresentKHR(queue, &present_info);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not present to queue: {}", format_vk_result(result));
        }
        return result;
    }
}
