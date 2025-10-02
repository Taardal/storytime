#pragma once

#include "st_vulkan_command_buffer.h"
#include "st_vulkan_device.h"

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

    public:
        VulkanBuffer(const Config& config);

        ~VulkanBuffer();

        operator VkBuffer() const;

        void set_data(const void* src) const;

        void copy_to(VkBuffer destination_buffer, const VulkanCommandBuffer& command_buffer) const;

    private:
        void create_buffer();

        void destroy_buffer() const;

        i32 get_memory_type_index(const VkMemoryRequirements& memory_requirements, VkMemoryPropertyFlags required_memory_properties) const;
    };
}
