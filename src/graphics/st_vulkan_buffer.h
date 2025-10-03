#pragma once

#include "graphics/st_vulkan_command_buffer.h"
#include "graphics/st_vulkan_device.h"

namespace Storytime {
    struct VulkanBufferConfig {
        VulkanDevice* device = nullptr;
        std::string name = "VulkanBuffer";
        VkDeviceSize size = 0;
        VkBufferUsageFlags usage = 0;
        VkMemoryPropertyFlags memory_properties = 0;
    };

    class VulkanBuffer {
    public:
        typedef VulkanBufferConfig Config;

    private:
        Config config;
        VkBuffer buffer = nullptr;
        VkDeviceMemory memory = nullptr;
        void* data = nullptr;

    public:
        VulkanBuffer(const Config& config);

        ~VulkanBuffer();

        VulkanBuffer(VulkanBuffer&& other) noexcept;

        VulkanBuffer(const VulkanBuffer& other) = delete;

        VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;

        VulkanBuffer& operator=(const VulkanBuffer& other) = delete;

        operator VkBuffer() const;

        void map_memory();

        void set_data(const void* src) const;

        void unmap_memory() const;

        void copy_to(VkBuffer destination_buffer, const VulkanCommandBuffer& command_buffer) const;

    private:
        void create_buffer();

        void destroy_buffer() const;

        void allocate_memory();

        void free_memory() const;

        i32 get_memory_type_index(const VkMemoryRequirements& memory_requirements, VkMemoryPropertyFlags required_memory_properties) const;
    };
}
