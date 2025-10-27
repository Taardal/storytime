#include "st_vulkan_vertex_buffer.h"

#include "graphics/st_vulkan_command_buffer.h"

namespace Storytime {
    VulkanVertexBuffer::VulkanVertexBuffer(const Config& config)
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

    VulkanVertexBuffer::~VulkanVertexBuffer() {
        if (staging_buffer_enabled) {
            staging_buffer.unmap_memory();
        } else {
            buffer.unmap_memory();
        }
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

    void VulkanVertexBuffer::bind(const VulkanCommandBuffer& command_buffer, const Bind& bind) const {
        VkDeviceSize offsets[] = {bind.offset};
        command_buffer.bind_vertex_buffer(bind.binding, buffer, offsets);
    }

    void VulkanVertexBuffer::bind(const VulkanCommandBuffer& command_buffer, u32 binding, VkDeviceSize offset) const {
        VkDeviceSize offsets[] = {offset};
        command_buffer.bind_vertex_buffer(binding, buffer, offsets);
    }

    void VulkanVertexBuffer::set_data(const void* data) const {
        ST_ASSERT(!staging_buffer_enabled, "Staging buffer must be disabled when setting vertex data without a command buffer");
        buffer.set_data(data);
    }

    void VulkanVertexBuffer::set_data(const VulkanCommandBuffer& command_buffer, const void* data) const {
        ST_ASSERT(staging_buffer_enabled, "Staging buffer must be enabled when setting vertex data with a command buffer");
        staging_buffer.set_data(data);
        staging_buffer.copy_data(command_buffer, buffer);
    }

    VulkanBuffer VulkanVertexBuffer::create_buffer() {
        return VulkanBuffer({
            .device = config.device,
            .name = std::format("{} buffer", config.name),
            .size = config.size,
            .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | config.usage,
            .memory_properties = config.memory_properties,
        });
    }

    VulkanBuffer VulkanVertexBuffer::create_staging_buffer() {
        return VulkanBuffer({
            .device = config.device,
            .name = std::format("{} staging buffer", config.name),
            .size = config.size,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        });
    }

    bool VulkanVertexBuffer::is_staging_buffer_enabled() const {
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
