#pragma once

#include "graphics/st_vulkan_buffer.h"
#include "graphics/st_vulkan_command_buffer.h"
#include "graphics/st_vulkan_device.h"

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
        VulkanBuffer staging_buffer;

    public:
        VulkanVertexBuffer(const Config& config);

        VulkanVertexBuffer(VulkanVertexBuffer&& other) noexcept;

        VulkanVertexBuffer(const VulkanVertexBuffer& other) = delete;

        VulkanVertexBuffer& operator=(VulkanVertexBuffer&& other) noexcept;

        VulkanVertexBuffer& operator=(const VulkanVertexBuffer& other) = delete;

        operator VkBuffer() const;

        void bind(const VulkanCommandBuffer& command_buffer, const VkDeviceSize* offsets = nullptr) const;

        void set_vertices(const void* vertices, const VulkanCommandBuffer& command_buffer);
    };
}