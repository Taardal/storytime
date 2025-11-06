#include "st_vulkan_buffer.h"

#include "graphics/st_vulkan_command_buffer.h"

namespace Storytime {
    VulkanBuffer::VulkanBuffer(const Config& config) : config(config.assert_valid()) {
        create_buffer();
        allocate_memory();
    }

    VulkanBuffer::~VulkanBuffer() {
        free_memory();
        destroy_buffer();
    }

    VulkanBuffer::VulkanBuffer(VulkanBuffer&& other) noexcept
        : config(std::move(other.config)),
          buffer(other.buffer),
          memory(other.memory),
          data(other.data)
    {
        other.buffer = nullptr;
        other.memory = nullptr;
    }

    VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other) noexcept {
        if (this != &other) {
            config = std::move(other.config);
            buffer = other.buffer;
            memory = other.memory;
            data = other.data;
            other.buffer = nullptr;
            other.memory = nullptr;
        }
        return *this;
    }

    VulkanBuffer::operator VkBuffer() const {
        return buffer;
    }

    void VulkanBuffer::map_memory() {
        ST_ASSERT_NOT_NULL(memory);
        constexpr VkDeviceSize offset = 0;
        constexpr VkMemoryMapFlags flags = 0;
        ST_ASSERT_THROW_VK(
            config.device->map_memory(memory, offset, config.size, flags, &data),
            "Could not map memory for buffer [" << config.name << "]"
        );
    }

    void VulkanBuffer::unmap_memory() const {
        if (memory != nullptr) {
            config.device->unmap_memory(memory);
        }
    }

    void VulkanBuffer::set_data(const void* src) const {
        memcpy(data, src, config.size);
    }

    void VulkanBuffer::copy_data(const VulkanCommandBuffer& command_buffer, VkBuffer destination_buffer) const {
        constexpr u32 copy_region_count = 1;

        VkBufferCopy copy_region{};
        copy_region.size = config.size;
        copy_region.srcOffset = 0;
        copy_region.dstOffset = 0;

        command_buffer.copy_buffer(buffer, destination_buffer, copy_region_count, &copy_region);
    }

    void VulkanBuffer::create_buffer() {
        const VulkanDevice& device = *config.device;

        VkBufferCreateInfo buffer_create_info{};
        buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_create_info.size = config.size;
        buffer_create_info.usage = config.usage;
        buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        ST_ASSERT_THROW_VK(
            device.create_buffer(buffer_create_info, &buffer, config.name),
            "Could not create buffer [" << config.name << "]"
        );
    }

    void VulkanBuffer::destroy_buffer() const {
        if (buffer != nullptr) {
            config.device->destroy_buffer(buffer);
        }
    }

    void VulkanBuffer::allocate_memory() {
        const VulkanDevice& device = *config.device;
        const VulkanPhysicalDevice& physical_device = device.get_physical_device();

        VkMemoryRequirements memory_requirements = device.get_buffer_memory_requirements(buffer);
        i32 memory_type_index = physical_device.find_supported_memory_type(memory_requirements, config.memory_properties);

        VkMemoryAllocateInfo memory_allocate_info{};
        memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memory_allocate_info.allocationSize = memory_requirements.size;
        memory_allocate_info.memoryTypeIndex = memory_type_index;

        std::string memory_name = std::format("{} memory", config.name);

        ST_ASSERT_THROW_VK(
            device.allocate_memory(memory_allocate_info, &memory, memory_name),
            "Could not allocate buffer memory [" << memory_name << "]"
        );

        ST_ASSERT_THROW_VK(
            device.bind_buffer_memory(buffer, memory),
            "Could not bind buffer memory [" << memory_name << "]"
        );
    }

    void VulkanBuffer::free_memory() const {
        if (memory != nullptr) {
            config.device->free_memory(memory);
        }
    }
}
