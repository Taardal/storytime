#pragma once

#include "st_vulkan_buffer.h"
#include "st_vulkan_command_buffer.h"
#include "st_vulkan_device.h"

namespace Storytime {
    struct VulkanIndexBufferConfig {
        VulkanDevice* device = nullptr;
        std::string name = "VulkanIndexBuffer";
        VkDeviceSize size = 0;
    };

    class VulkanIndexBuffer {
    public:
        typedef VulkanIndexBufferConfig Config;

    private:
        Config config;
        VulkanBuffer buffer;
        VulkanBuffer staging_buffer;

    public:
        VulkanIndexBuffer(const Config& config);

        operator VkBuffer() const;

        void bind(const VulkanCommandBuffer& command_buffer, VkIndexType index_type, VkDeviceSize offset = 0) const;

        void set_indices(const void* indices, const VulkanCommandBuffer& command_buffer) const;
    };
}