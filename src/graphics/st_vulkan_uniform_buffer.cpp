#include "st_vulkan_uniform_buffer.h"

namespace Storytime {
    VulkanUniformBuffer::VulkanUniformBuffer(const Config& config)
        : config(config.assert_valid()),
          buffer(create_buffer())
    {
        buffer.map_memory();
    }

    VulkanUniformBuffer::VulkanUniformBuffer(VulkanUniformBuffer&& other) noexcept
        : config(std::move(other.config)),
          buffer(std::move(other.buffer)) {}

    VulkanUniformBuffer& VulkanUniformBuffer::operator=(VulkanUniformBuffer&& other) noexcept {
        if (this != &other) {
            config = std::move(other.config);
            buffer = std::move(other.buffer);
        }
        return *this;
    }

    VulkanUniformBuffer::operator VkBuffer() const {
        return buffer;
    }

    void VulkanUniformBuffer::set_data(const void* data) const {
        buffer.set_data(data);
    }

    VulkanBuffer VulkanUniformBuffer::create_buffer() {
        return VulkanBuffer({
            .device = config.device,
            .name = std::format("{} buffer", config.name),
            .size = config.size,
            .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            .memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        });
    }
}
