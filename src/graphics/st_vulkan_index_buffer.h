#pragma once

#include "graphics/st_vulkan_buffer.h"
#include "graphics/st_vulkan_command_buffer.h"
#include "graphics/st_vulkan_device.h"

namespace Storytime {
    struct VulkanIndexBufferConfig {
        std::string name = "VulkanIndexBuffer";
        VulkanDevice* device = nullptr;
        VkIndexType index_type = VK_INDEX_TYPE_MAX_ENUM;
        VkDeviceSize size = 0;
        VkBufferUsageFlags usage = 0;
        VkMemoryPropertyFlags memory_properties = 0;

        const VulkanIndexBufferConfig& assert_valid() const {
            ST_ASSERT_NOT_NULL(device);
            ST_ASSERT_GREATER_THAN_ZERO(size);
            ST_ASSERT_NOT_EQUAL(index_type, VK_INDEX_TYPE_MAX_ENUM);
            return *this;
        }
    };

    class VulkanIndexBuffer {
    public:
        typedef VulkanIndexBufferConfig Config;

    private:
        Config config{};
        VulkanBuffer buffer{};
        VulkanBuffer staging_buffer{};
        bool staging_buffer_enabled = false;

    public:
        VulkanIndexBuffer() = default;

        VulkanIndexBuffer(const Config& config);

        ~VulkanIndexBuffer();

        VulkanIndexBuffer(VulkanIndexBuffer&& other) noexcept;

        VulkanIndexBuffer(const VulkanIndexBuffer& other) = delete;

        VulkanIndexBuffer& operator=(VulkanIndexBuffer&& other) noexcept;

        VulkanIndexBuffer& operator=(const VulkanIndexBuffer& other) = delete;

        operator VkBuffer() const;

        void bind(const VulkanCommandBuffer& command_buffer, VkDeviceSize offset = 0) const;

        void set_data(const void* data) const;

        void set_data(const VulkanCommandBuffer& command_buffer, const void* data) const;

    private:
        VulkanBuffer create_buffer();

        VulkanBuffer create_staging_buffer();

        bool is_staging_buffer_enabled() const;
    };
}