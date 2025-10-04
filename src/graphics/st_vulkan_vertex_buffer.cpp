#include "st_vulkan_vertex_buffer.h"

#include "graphics/st_vulkan_command_buffer.h"

namespace Storytime {
    VulkanVertexBuffer::VulkanVertexBuffer(const Config& config)
        : config(config),
          buffer({
              .device = config.device,
              .name = std::format("{} buffer", config.name),
              .size = config.size,
              .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
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
        staging_buffer.map_memory();
    }

    VulkanVertexBuffer::VulkanVertexBuffer(VulkanVertexBuffer&& other) noexcept
        : config(std::move(other.config)),
          buffer(std::move(other.buffer)),
          staging_buffer(std::move(other.staging_buffer)) {}

    VulkanVertexBuffer& VulkanVertexBuffer::operator=(VulkanVertexBuffer&& other) noexcept {
        if (this != &other) {
            config = std::move(other.config);
            buffer = std::move(other.buffer);
            staging_buffer = std::move(other.staging_buffer);
        }
        return *this;
    }

    VulkanVertexBuffer::operator VkBuffer() const {
        return buffer;
    }

    void VulkanVertexBuffer::bind(const VulkanCommandBuffer& command_buffer, const VkDeviceSize* offsets) const {
        static VkDeviceSize default_offsets[] = {0};
        command_buffer.bind_vertex_buffer(buffer, offsets == nullptr ? default_offsets : offsets);
    }

    void VulkanVertexBuffer::set_vertices(const void* vertices, const VulkanCommandBuffer& command_buffer) const {
        staging_buffer.set_data(vertices);
        staging_buffer.copy_to(buffer, command_buffer);
    }
}
