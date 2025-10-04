#include "st_vulkan_descriptor_set.h"

namespace Storytime {
    VulkanDescriptorSet::VulkanDescriptorSet(VkDescriptorSet descriptor_set) : descriptor_set(descriptor_set) {}

    VulkanDescriptorSet::operator VkDescriptorSet() const {
        return descriptor_set;
    }

    void VulkanDescriptorSet::bind(const VulkanCommandBuffer& command_buffer, VkPipelineLayout pipeline_layout, VkPipelineBindPoint pipeline_bind_point) const {
        u32 dynamic_offset_count = 0;
        const u32* dynamic_offsets = nullptr;
        command_buffer.bind_descriptor_set(pipeline_bind_point, pipeline_layout, descriptor_set, dynamic_offset_count, dynamic_offsets);
    }

    void VulkanDescriptorSet::write(const VulkanDevice& device, u32 descriptor_write_count, const VkWriteDescriptorSet* descriptor_writes) const {
        u32 descriptor_copy_count = 0;
        VkCopyDescriptorSet* descriptor_copies = nullptr;
        device.update_descriptor_sets(descriptor_write_count, descriptor_writes, descriptor_copy_count, descriptor_copies);
    }

    void VulkanDescriptorSet::write(const VulkanDevice& device, const std::vector<VkWriteDescriptorSet>& descriptor_writes) const {
        write(device, descriptor_writes.size(), descriptor_writes.data());
    }

    void VulkanDescriptorSet::write(const VulkanDevice& device, const VkWriteDescriptorSet& descriptor_write) const {
        write(device, 1, &descriptor_write);
    }

    void VulkanDescriptorSet::copy(const VulkanDevice& device, u32 descriptor_copy_count, const VkCopyDescriptorSet* descriptor_copies) const {
        u32 descriptor_write_count = 0;
        VkWriteDescriptorSet* descriptor_writes = nullptr;
        device.update_descriptor_sets(descriptor_write_count, descriptor_writes, descriptor_copy_count, descriptor_copies);
    }

    void VulkanDescriptorSet::copy(const VulkanDevice& device, const VkCopyDescriptorSet& descriptor_copy) const {
        copy(device, 1, &descriptor_copy);
    }
}
