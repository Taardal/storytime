#pragma once

#include "graphics/st_vulkan_buffer.h"
#include "graphics/st_vulkan_command_buffer.h"
#include "graphics/st_vulkan_device.h"

namespace Storytime {
    struct VulkanVertexBufferConfig {
        std::string name = "VulkanVertexBuffer";
        VulkanDevice* device = nullptr;
        VkDeviceSize size = 0;
        VkBufferUsageFlags usage = 0;
        VkMemoryPropertyFlags memory_properties = 0;

        const VulkanVertexBufferConfig& assert_valid() const {
            ST_ASSERT_NOT_NULL(device);
            ST_ASSERT_GREATER_THAN_ZERO(size);
            return *this;
        }
    };

    class VulkanVertexBuffer {
    public:
        typedef VulkanVertexBufferConfig Config;

    private:
        Config config{};
        VulkanBuffer buffer{};
        VulkanBuffer staging_buffer{};
        bool staging_buffer_enabled = false;

    public:
        VulkanVertexBuffer() = default;

        VulkanVertexBuffer(const Config& config);

        ~VulkanVertexBuffer();

        VulkanVertexBuffer(VulkanVertexBuffer&& other) noexcept;

        VulkanVertexBuffer(const VulkanVertexBuffer& other) = delete;

        VulkanVertexBuffer& operator=(VulkanVertexBuffer&& other) noexcept;

        VulkanVertexBuffer& operator=(const VulkanVertexBuffer& other) = delete;

        operator VkBuffer() const;

        struct Bind {
            u32 binding = 0;
            VkDeviceSize offset = 0;
        };

        void bind(const VulkanCommandBuffer& command_buffer, const Bind& bind) const;

        void bind(const VulkanCommandBuffer& command_buffer, u32 binding = 0, VkDeviceSize offset = 0) const;

        void set_data(const void* data) const;

        void set_data(const VulkanCommandBuffer& command_buffer, const void* data) const;

    private:
        VulkanBuffer create_buffer();

        VulkanBuffer create_staging_buffer();

        bool is_staging_buffer_enabled() const;
    };
}