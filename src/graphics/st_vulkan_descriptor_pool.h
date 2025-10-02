#pragma once

#include "st_vulkan_device.h"
#include "st_vulkan_physical_device.h"

namespace Storytime {
    struct VulkanDescriptorPoolConfig {
        VulkanDevice* device = nullptr;
        std::string name = "VulkanDescriptorPool";
        u32 descriptor_count = 0;
        u32 max_descriptor_sets = 0;
        VkDescriptorType descriptor_type = VK_DESCRIPTOR_TYPE_MAX_ENUM;
        VkDescriptorPoolSize descriptor_pool_size{};
    };

    class VulkanDescriptorPool {
    public:
        typedef VulkanDescriptorPoolConfig Config;

    private:
        Config config;
        VkDescriptorPool descriptor_pool = nullptr;

    public:
        VulkanDescriptorPool(const Config& config);

        ~VulkanDescriptorPool();

        struct DescriptorSetAllocationConfig {
            std::string name = "DescriptorSet";
            u32 count = 0;
        };

        void allocate_descriptor_sets(u32 descriptor_set_count, const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts, VkDescriptorSet* descriptor_sets) const;

        std::vector<VkDescriptorSet> allocate_descriptor_sets(u32 descriptor_set_count, const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts) const;

        void allocate_descriptor_set(VkDescriptorSet* descriptor_set) const;

        VkDescriptorSet allocate_descriptor_set() const;

    private:
        void create_descriptor_pool();

        void destroy_descriptor_pool() const;
    };
}