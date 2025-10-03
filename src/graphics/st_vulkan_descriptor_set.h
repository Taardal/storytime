#pragma once

#include "st_vulkan_command_buffer.h"
#include "st_vulkan_device.h"

namespace Storytime {
    class VulkanDescriptorSet {
    private:
        VkDescriptorSet descriptor_set = nullptr;

    public:
        VulkanDescriptorSet() = default;

        VulkanDescriptorSet(VkDescriptorSet descriptor_set);

        operator VkDescriptorSet() const;

        void bind(const VulkanCommandBuffer& command_buffer, VkPipelineLayout pipeline_layout, VkPipelineBindPoint pipeline_bind_point) const;

        void write(const VulkanDevice& device, u32 descriptor_write_count, const VkWriteDescriptorSet* descriptor_writes) const;

        void write(const VulkanDevice& device, const VkWriteDescriptorSet& descriptor_write) const;

        void copy(const VulkanDevice& device, u32 descriptor_copy_count, const VkCopyDescriptorSet* descriptor_copies) const;

        void copy(const VulkanDevice& device, const VkCopyDescriptorSet& descriptor_copy) const;
    };
}
