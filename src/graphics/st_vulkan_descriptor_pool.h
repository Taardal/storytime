#pragma once

#include "graphics/st_vulkan_device.h"
#include "graphics/st_vulkan_physical_device.h"

namespace Storytime {
    struct VulkanDescriptorPoolConfig {
        VulkanDevice* device = nullptr;
        std::string name = "VulkanDescriptorPool";
        u32 max_descriptor_sets = 0;
        std::vector<VkDescriptorPoolSize> descriptor_pool_sizes{};
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

        void allocate_descriptor_sets(u32 descriptor_set_count, VkDescriptorSet* descriptor_sets, const VkDescriptorSetLayout* descriptor_set_layouts) const;

        std::vector<VkDescriptorSet> allocate_descriptor_sets(u32 descriptor_set_count, const VkDescriptorSetLayout* descriptor_set_layouts) const;

        void allocate_descriptor_set(VkDescriptorSet* descriptor_set, VkDescriptorSetLayout descriptor_set_layout) const;

        VkDescriptorSet allocate_descriptor_set(VkDescriptorSetLayout descriptor_set_layout) const;

    private:
        void create_descriptor_pool();

        void destroy_descriptor_pool() const;
    };
}