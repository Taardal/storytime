#include "graphics/st_vulkan_uniform_buffer.h"

namespace Storytime {
    VulkanUniformBuffer::VulkanUniformBuffer(const Config& config)
        : config(config),
          buffer({
              .device = config.device,
              .name = std::format("{} buffer", config.name),
              .size = config.size,
              .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
              .memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
          })
    {
        // Keep the memory mapped for the uniform buffer's whole lifetime (a.k.a. "persistent mapping")
        // Not having to map the buffer every time we need to update it increases performances, as mapping is not free.
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

    void VulkanUniformBuffer::set_uniforms(const void* uniforms) const {
        buffer.set_data(uniforms);
    }
}
