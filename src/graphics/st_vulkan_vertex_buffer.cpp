#include "st_vulkan_vertex_buffer.h"

#include "st_vulkan_command_buffer.h"

namespace Storytime {
    VulkanVertexBuffer::VulkanVertexBuffer(const Config& config)
        : config(config),
          buffer({
              .device = config.device,
              .name = std::format("{} (internal)", config.name),
              .size = config.size,
              .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
              .memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
          })
    {
    }

    VulkanVertexBuffer::operator VkBuffer() const {
        return buffer;
    }

    void VulkanVertexBuffer::bind(const VulkanCommandBuffer& command_buffer) const {
        VkDeviceSize offsets[] = {0};
        command_buffer.bind_vertex_buffer(buffer, offsets);
    }

    void VulkanVertexBuffer::set_data(const void* data) const {
        buffer.set_data(data);
    }
}
