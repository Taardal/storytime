#pragma once

namespace Storytime {
    class VulkanQueue {
    private:
        VkQueue queue = nullptr;

    public:
        VulkanQueue() = default;

        VulkanQueue(VkQueue queue);

        operator VkQueue() const;

        struct SubmitConfig {
            VkQueue queue = nullptr;
            u32 submit_count = 0;
            VkSubmitInfo* submit_info = nullptr;
            VkFence fence = nullptr;
        };

        VkResult submit(const SubmitConfig& config) const;

        VkResult submit(const VkSubmitInfo& submit_info, VkFence fence = nullptr) const;

        VkResult submit(u32 submit_count, const VkSubmitInfo* submit_info, VkFence fence = nullptr) const;

        VkResult present(const VkPresentInfoKHR& present_info) const;
    };
}
