#include "graphics/st_vulkan_index_buffer.h"

#include "graphics/st_vulkan_command_buffer.h"

namespace Storytime {
    VulkanIndexBuffer::VulkanIndexBuffer(const Config& config)
        : config(config),
          buffer({
              .device = config.device,
              .name = std::format("{} buffer", config.name),
              .size = config.size,
              .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
              .memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
          }),
          staging_buffer({
            .device = config.device,
            .name = std::format("{} staging buffer", config.name),
            .size = config.size,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
          })
    {
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

    void VulkanIndexBuffer::bind(const VulkanCommandBuffer& command_buffer, VkIndexType index_type, VkDeviceSize offset) const {
        command_buffer.bind_index_buffer(buffer, offset, index_type);
    }

    void VulkanIndexBuffer::set_indices(const void* indices, const VulkanCommandBuffer& command_buffer) {
        staging_buffer.map_memory();
        staging_buffer.set_data(indices);
        staging_buffer.unmap_memory();
        staging_buffer.copy_to(buffer, command_buffer);
    }
}
