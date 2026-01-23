#pragma once

#include "graphics/st_vulkan_device.h"

namespace Storytime {
    struct VulkanDescriptorPoolConfig {
        std::string name = "VulkanDescriptorPool";
        const VulkanDevice& device;
        u32 max_descriptor_sets = 0;
        std::vector<VkDescriptorPoolSize> descriptor_pool_sizes{};

        const VulkanDescriptorPoolConfig& assert_valid() const {
            ST_ASSERT_GREATER_THAN_ZERO(max_descriptor_sets);
            ST_ASSERT_NOT_EMPTY(descriptor_pool_sizes);
            return *this;
        }
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

        void allocate_descriptor_set(VkDescriptorSet* descriptor_set, VkDescriptorSetLayout descriptor_set_layout, std::string_view name = "") const;

        VkDescriptorSet allocate_descriptor_set(VkDescriptorSetLayout descriptor_set_layout, std::string_view name = "") const;

    private:
        void create_descriptor_pool();

        void destroy_descriptor_pool() const;
    };
}
