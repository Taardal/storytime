#include "st_vulkan_uniform_buffer.h"

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
        buffer.map_memory(&dst);
        ST_LOG_C("CREATED UNIFORM BUFFER");
    }

    VulkanUniformBuffer::~VulkanUniformBuffer() {
        // buffer.unmap_memory();
    }

    VulkanUniformBuffer::operator VkBuffer() const {
        return buffer;
    }

    void VulkanUniformBuffer::set_uniforms(const void* uniforms) const {
        buffer.set_memory_data(uniforms, dst);
    }
}
