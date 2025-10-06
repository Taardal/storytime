#include "st_vulkan_queue.h"

namespace Storytime {
    VulkanQueue::VulkanQueue(VkQueue queue) : queue(queue) {}

    VulkanQueue::operator VkQueue() const {
        return queue;
    }

    VkResult VulkanQueue::submit(const SubmitConfig& config) const {
        return vkQueueSubmit(config.queue, config.submit_count, config.submit_info, config.fence);
    }

    VkResult VulkanQueue::submit(const VkSubmitInfo& submit_info, VkFence fence) const {
        u32 submit_count = 1;
        return vkQueueSubmit(queue, submit_count, &submit_info, fence);
    }

    VkResult VulkanQueue::submit(u32 submit_count, const VkSubmitInfo* submit_info, VkFence fence) const {
        return vkQueueSubmit(queue, submit_count, submit_info, fence);
    }

    VkResult VulkanQueue::submit(VkCommandBuffer command_buffer) const {
        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer;
        return submit(submit_info);
    }

    VkResult VulkanQueue::present(const VkPresentInfoKHR& present_info) const {
        return vkQueuePresentKHR(queue, &present_info);
    }
}
