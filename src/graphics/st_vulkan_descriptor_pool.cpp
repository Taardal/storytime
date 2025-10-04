#include "st_vulkan_descriptor_pool.h"

namespace Storytime {
    VulkanDescriptorPool::VulkanDescriptorPool(const Config& config) : config(config) {
        create_descriptor_pool();
    }

    VulkanDescriptorPool::~VulkanDescriptorPool() {
        destroy_descriptor_pool();
    }

    void VulkanDescriptorPool::allocate_descriptor_sets(u32 descriptor_set_count, VkDescriptorSet* descriptor_sets, const VkDescriptorSetLayout* descriptor_set_layouts) const {
        VkDescriptorSetAllocateInfo descriptor_set_allocate_info{};
        descriptor_set_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptor_set_allocate_info.descriptorPool = descriptor_pool;
        descriptor_set_allocate_info.descriptorSetCount = descriptor_set_count;
        descriptor_set_allocate_info.pSetLayouts = descriptor_set_layouts;

        if (config.device->allocate_descriptor_sets(descriptor_set_allocate_info, descriptor_sets) != VK_SUCCESS) {
            ST_THROW("Could not allocate [" << descriptor_set_count << "] descriptor sets");
        }
    }

    std::vector<VkDescriptorSet> VulkanDescriptorPool::allocate_descriptor_sets(u32 descriptor_set_count, const VkDescriptorSetLayout* descriptor_set_layouts) const {
        std::vector<VkDescriptorSet> descriptor_sets(descriptor_set_count);
        allocate_descriptor_sets(descriptor_set_count, descriptor_sets.data(), descriptor_set_layouts);
        return descriptor_sets;
    }

    void VulkanDescriptorPool::allocate_descriptor_set(VkDescriptorSet* descriptor_set, VkDescriptorSetLayout descriptor_set_layout) const {
        allocate_descriptor_sets(1, descriptor_set, &descriptor_set_layout);
    }

    VkDescriptorSet VulkanDescriptorPool::allocate_descriptor_set(VkDescriptorSetLayout descriptor_set_layout) const {
        VkDescriptorSet descriptor_set;
        allocate_descriptor_sets(1, &descriptor_set, &descriptor_set_layout);
        return descriptor_set;
    }

    void VulkanDescriptorPool::create_descriptor_pool() {
        VkDescriptorPoolCreateInfo descriptor_pool_create_info{};
        descriptor_pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptor_pool_create_info.poolSizeCount = config.descriptor_pool_sizes.size();
        descriptor_pool_create_info.pPoolSizes = config.descriptor_pool_sizes.data();
        descriptor_pool_create_info.maxSets = config.max_descriptor_sets;

        if (config.device->create_descriptor_pool(descriptor_pool_create_info, &descriptor_pool) != VK_SUCCESS) {
            ST_THROW("Could not create descriptor pool [" << config.name << "]");
        }
        if (config.device->set_object_name(descriptor_pool, VK_OBJECT_TYPE_DESCRIPTOR_POOL, config.name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set descriptor pool name [" << config.name << "]");
        }
    }

    void VulkanDescriptorPool::destroy_descriptor_pool() const {
        config.device->destroy_descriptor_pool(descriptor_pool);
    }
}
