#pragma once

#include "st_vulkan_buffer.h"
#include "st_vulkan_command_buffer.h"
#include "st_vulkan_device.h"

namespace Storytime {
    struct VulkanVertexBufferConfig {
        VulkanDevice* device = nullptr;
        std::string name = "VulkanVertexBuffer";
        VkDeviceSize size = 0;
    };

    class VulkanVertexBuffer {
    public:
        typedef VulkanVertexBufferConfig Config;

    private:
        Config config;
        VulkanBuffer buffer;

    public:
        VulkanVertexBuffer(const Config& config);

        operator VkBuffer() const;

        void bind(const VulkanCommandBuffer& command_buffer) const;

        void set_data(const void* data) const;
    };
}