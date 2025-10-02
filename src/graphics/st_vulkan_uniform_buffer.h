#pragma once

#include "st_vulkan_buffer.h"
#include "st_vulkan_device.h"

namespace Storytime {
    struct VulkanUniformBufferConfig {
        VulkanDevice* device = nullptr;
        std::string name = "VulkanUniformBuffer";
        VkDeviceSize size = 0;
    };

    class VulkanUniformBuffer {
    public:
        typedef VulkanUniformBufferConfig Config;

    private:
        Config config;
        VulkanBuffer buffer;
        void* dst;

    public:
        VulkanUniformBuffer(const Config& config);

        ~VulkanUniformBuffer();

        operator VkBuffer() const;

        void set_uniforms(const void* uniforms) const;
    };
}