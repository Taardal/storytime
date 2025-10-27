#include "graphics/st_vulkan_index_buffer.h"

#include "graphics/st_vulkan_command_buffer.h"

namespace Storytime {
    VulkanIndexBuffer::VulkanIndexBuffer(const Config& config)
        : config(config.assert_valid()),
          buffer(create_buffer())
    {
        staging_buffer_enabled = is_staging_buffer_enabled();
        if (staging_buffer_enabled) {
            staging_buffer = create_staging_buffer();
            staging_buffer.map_memory();
        } else {
            buffer.map_memory();
        }
    }

    VulkanIndexBuffer::~VulkanIndexBuffer() {
        if (staging_buffer_enabled) {
            staging_buffer.unmap_memory();
        } else {
            buffer.unmap_memory();
        }
    }

    VulkanIndexBuffer::VulkanIndexBuffer(VulkanIndexBuffer&& other) noexcept
        : config(std::move(other.config)),
          buffer(std::move(other.buffer)),
          staging_buffer(std::move(other.staging_buffer)) {}

    VulkanIndexBuffer& VulkanIndexBuffer::operator=(VulkanIndexBuffer&& other) noexcept {
        if (this != &other) {
            config = std::move(other.config);
            buffer = std::move(other.buffer);
            staging_buffer = std::move(other.staging_buffer);
        }
        return *this;
    }

    VulkanIndexBuffer::operator VkBuffer() const {
        return buffer;
    }

    void VulkanIndexBuffer::bind(const VulkanCommandBuffer& command_buffer, VkDeviceSize offset) const {
        command_buffer.bind_index_buffer(buffer, offset, config.index_type);
    }

    void VulkanIndexBuffer::set_data(const void* data) const {
        ST_ASSERT(!staging_buffer_enabled, "Staging buffer must be disabled when setting index data without a command buffer");
        buffer.set_data(data);
    }

    void VulkanIndexBuffer::set_data(const VulkanCommandBuffer& command_buffer, const void* data) const {
        ST_ASSERT(staging_buffer_enabled, "Staging buffer must be enabled when setting index data with a command buffer");
        staging_buffer.set_data(data);
        staging_buffer.copy_data(command_buffer, buffer);
    }

    VulkanBuffer VulkanIndexBuffer::create_buffer() {
        return VulkanBuffer({
            .device = config.device,
            .name = std::format("{} buffer", config.name),
            .size = config.size,
            .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | config.usage,
            .memory_properties = config.memory_properties,
        });
    }

    VulkanBuffer VulkanIndexBuffer::create_staging_buffer() {
        return VulkanBuffer({
            .device = config.device,
            .name = std::format("{} staging buffer", config.name),
            .size = config.size,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        });
    }

    bool VulkanIndexBuffer::is_staging_buffer_enabled() const {
        bool primary_buffer_will_be_copied_to = config.usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        if (!primary_buffer_will_be_copied_to) {
            return false;
        }
        bool primary_buffer_memory_is_only_visible_to_gpu = config.memory_properties & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        if (!primary_buffer_memory_is_only_visible_to_gpu) {
            return false;
        }
        return true;
    }
}
