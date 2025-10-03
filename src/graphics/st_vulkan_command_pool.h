#pragma once

#include "graphics/st_vulkan_device.h"

namespace Storytime {
    struct VulkanCommandPoolConfig {
        VulkanDevice* device = nullptr;
        std::string name = "VulkanCommandPool";
        u32 queue_family_index = 0;
        VkCommandPoolCreateFlags flags = 0;
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

        void allocate_command_buffer(VkCommandBuffer* command_buffer) const;

        VkCommandBuffer allocate_command_buffer() const;

        void free_command_buffers(u32 command_buffer_count, VkCommandBuffer* command_buffers) const;

        void free_command_buffer(VkCommandBuffer command_buffer) const;

    private:
        void create_command_pool();

        void destroy_command_pool() const;
    };
}
