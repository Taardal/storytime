#pragma once

#include "st_vulkan_device.h"
#include "st_vulkan_physical_device.h"

namespace Storytime {
    struct VulkanCommandPoolConfig {
        VulkanPhysicalDevice* physical_device = nullptr;
        VulkanDevice* device = nullptr;
        std::string name = "VulkanCommandPool";
    };

    class VulkanCommandPool {
    public:
        typedef VulkanCommandPoolConfig Config;

    private:
        Config config;
        VkCommandPool command_pool;

    public:
        VulkanCommandPool(const Config& config);

        ~VulkanCommandPool();

        struct CommandBufferAllocationConfig {
            std::string name = "CommandBuffer";
            u32 count = 0;
        };

        void allocate_command_buffers(u32 command_buffer_count, VkCommandBuffer* command_buffers) const;

        std::vector<VkCommandBuffer> allocate_command_buffers(u32 command_buffer_count) const;

    private:
        void create_command_pool();

        void destroy_command_pool() const;
    };
}
