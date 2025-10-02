#include "st_vulkan_descriptor_pool.h"

namespace Storytime {
    VulkanDescriptorPool::VulkanDescriptorPool(const Config& config) : config(config) {
        create_descriptor_pool();
    }

    VulkanDescriptorPool::~VulkanDescriptorPool() {
        destroy_descriptor_pool();
    }

    void VulkanDescriptorPool::allocate_descriptor_sets(u32 descriptor_set_count, const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts, VkDescriptorSet* descriptor_sets) const {
        VkDescriptorSetAllocateInfo descriptor_set_allocate_info{};
        descriptor_set_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptor_set_allocate_info.descriptorPool = descriptor_pool;
        descriptor_set_allocate_info.descriptorSetCount = descriptor_set_count;
        descriptor_set_allocate_info.pSetLayouts = descriptor_set_layouts.data();

        if (config.device->allocate_descriptor_sets(descriptor_set_allocate_info, descriptor_sets) != VK_SUCCESS) {
            ST_THROW("Could not allocate [" << descriptor_set_count << "] descriptor sets");
        }
    }

    std::vector<VkDescriptorSet> VulkanDescriptorPool::allocate_descriptor_sets(u32 descriptor_set_count, const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts) const {
        std::vector<VkDescriptorSet> descriptor_sets(descriptor_set_count);
        allocate_descriptor_sets(descriptor_set_count, descriptor_set_layouts, descriptor_sets.data());
        return descriptor_sets;
    }

    void VulkanDescriptorPool::create_descriptor_pool() {
        VkDescriptorPoolSize descriptor_pool_size{};
        descriptor_pool_size.type = config.descriptor_type;
        descriptor_pool_size.descriptorCount = config.descriptor_count;

        VkDescriptorPoolCreateInfo descriptor_pool_create_info{};
        descriptor_pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptor_pool_create_info.poolSizeCount = 1;
        descriptor_pool_create_info.pPoolSizes = &descriptor_pool_size;
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
