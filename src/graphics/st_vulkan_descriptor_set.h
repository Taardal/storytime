#pragma once

#include "st_vulkan_command_buffer.h"
#include "st_vulkan_device.h"

namespace Storytime {
    class VulkanDescriptorSet {
    private:
        VkDescriptorSet descriptor_set = nullptr;

    public:
        VulkanDescriptorSet(VkDescriptorSet descriptor_set);

        operator VkDescriptorSet() const;

        struct Bind {
            VkPipelineBindPoint pipeline_bind_point = VK_PIPELINE_BIND_POINT_MAX_ENUM;
            VkPipelineLayout pipeline_layout = nullptr;
            u32 set = 0;
        };

        void bind(const VulkanCommandBuffer& command_buffer, const Bind& bind) const;

        void bind(const VulkanCommandBuffer& command_buffer, VkPipelineBindPoint pipeline_bind_point, VkPipelineLayout pipeline_layout, u32 first_set = 0) const;

        void write(const VulkanDevice& device, u32 descriptor_write_count, const VkWriteDescriptorSet* descriptor_writes) const;

        void write(const VulkanDevice& device, const std::vector<VkWriteDescriptorSet>& descriptor_writes) const;

        void write(const VulkanDevice& device, const VkWriteDescriptorSet& descriptor_write) const;

        void copy(const VulkanDevice& device, u32 descriptor_copy_count, const VkCopyDescriptorSet* descriptor_copies) const;

        void copy(const VulkanDevice& device, const VkCopyDescriptorSet& descriptor_copy) const;
    };
}
