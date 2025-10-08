#pragma once

namespace Storytime {
    class VulkanQueue {
    private:
        VkQueue queue = nullptr;

    public:
        VulkanQueue(VkQueue queue);

        operator VkQueue() const;

        VkResult submit(u32 submit_count, const VkSubmitInfo& submit_info, VkFence fence = nullptr) const;

        struct SubmitConfig {
            u32 submit_count = 0;
            VkSubmitInfo submit_info{};
            VkFence fence = nullptr;
        };

        VkResult submit(const SubmitConfig& config) const;

        VkResult submit(const VkSubmitInfo& submit_info, VkFence fence = nullptr) const;

        VkResult submit(u32 command_buffer_count, const VkCommandBuffer* command_buffers, VkFence fence = nullptr) const;

        VkResult submit(const std::vector<VkCommandBuffer>& command_buffers, VkFence fence = nullptr) const;

        VkResult submit(VkCommandBuffer command_buffer, VkFence fence = nullptr) const;

        VkResult present(const VkPresentInfoKHR& present_info) const;
    };
}
