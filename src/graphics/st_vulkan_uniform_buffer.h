#pragma once

#include "graphics/st_vulkan_buffer.h"
#include "graphics/st_vulkan_device.h"

namespace Storytime {
    struct VulkanUniformBufferConfig {
        std::string name = "VulkanUniformBuffer";
        VulkanDevice* device = nullptr;
        VkDeviceSize size = 0;

        const VulkanUniformBufferConfig& assert_valid() const {
            ST_ASSERT_NOT_NULL(device);
            ST_ASSERT_GREATER_THAN_ZERO(size);
            return *this;
        }
    };

    class VulkanUniformBuffer {
    public:
        typedef VulkanUniformBufferConfig Config;

    private:
        Config config{};
        VulkanBuffer buffer{};

    public:
        VulkanUniformBuffer() = default;

        VulkanUniformBuffer(const Config& config);

        VulkanUniformBuffer(VulkanUniformBuffer&& other) noexcept;

        VulkanUniformBuffer(const VulkanUniformBuffer& other) = delete;

        VulkanUniformBuffer& operator=(VulkanUniformBuffer&& other) noexcept;

        VulkanUniformBuffer& operator=(const VulkanUniformBuffer& other) = delete;

        operator VkBuffer() const;

        void set_data(const void* data) const;

    private:
        VulkanBuffer create_buffer();
    };
}