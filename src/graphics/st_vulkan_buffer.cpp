#include "st_vulkan_buffer.h"

#include "st_vulkan_command_buffer.h"
#include "st_vulkan_queue.h"

namespace Storytime {
    VulkanBuffer::VulkanBuffer(const Config& config) : config(config) {
        create_buffer();
    }

    VulkanBuffer::~VulkanBuffer() {
        destroy_buffer();
    }

    VulkanBuffer::operator VkBuffer() const {
        return buffer;
    }

    void VulkanBuffer::set_data(const void* src) const {
        const VulkanDevice& device = *config.device;

        // Map the buffer memory (graphics card memory) into CPU accessible memory
        void* dst;
        VkDeviceSize offset = 0;
        VkMemoryMapFlags flags = 0;
        if (device.map_memory(memory, offset, config.size, flags, &dst) != VK_SUCCESS) {
            ST_THROW("Could not map memory");
        }

        // Copy the data to the buffer memory (graphics card memory) which is now accessible by the CPU.
        memcpy(dst, src, config.size);

        // Unmap the memory when the data has been copied.
        device.unmap_memory(memory);
    }

    void VulkanBuffer::copy_to(VkBuffer destination_buffer, const VulkanCommandBuffer& command_buffer) const {
        VkBufferCopy copy_region{};
        copy_region.size = config.size;
        copy_region.srcOffset = 0;
        copy_region.dstOffset = 0;

        u32 copy_region_count = 1;
        command_buffer.copy_buffer(buffer, destination_buffer, copy_region_count, &copy_region);
    }

    void VulkanBuffer::create_buffer() {
        const VulkanDevice& device = *config.device;

        VkBufferCreateInfo buffer_create_info{};
        buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_create_info.size = config.size;
        buffer_create_info.usage = config.usage;
        buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (device.create_buffer(buffer_create_info, &buffer) != VK_SUCCESS) {
            ST_THROW("Could not create buffer");
        }

        if (device.set_object_name(buffer, VK_OBJECT_TYPE_BUFFER, config.name.c_str())) {
            ST_THROW("Could not set buffer name [" << config.name << "]");
        }

        VkMemoryRequirements buffer_memory_requirements = device.get_buffer_memory_requirements(buffer);

        u32 memory_type_index = get_memory_type_index(buffer_memory_requirements, config.memory_properties);

        VkMemoryAllocateInfo memory_allocate_info{};
        memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memory_allocate_info.allocationSize = buffer_memory_requirements.size;
        memory_allocate_info.memoryTypeIndex = memory_type_index;

        if (device.allocate_memory(memory_allocate_info, &memory) != VK_SUCCESS) {
            ST_THROW("Could not allocate buffer memory");
        }

        if (device.bind_buffer_memory(buffer, memory) != VK_SUCCESS) {
            ST_THROW("Could not bind buffer memory");
        }
    }

    void VulkanBuffer::destroy_buffer() const {
        const VulkanDevice& device = *config.device;
        device.destroy_buffer(buffer);
        device.free_memory(memory); // Memory that is bound to a buffer object may be freed once the buffer is no longer used.
    }

    // Graphics cards can offer different types of memory to allocate from. Each type of memory varies in terms of allowed operations
    // and performance characteristics. We need to combine the requirements of the buffer and our own application requirements to find
    // the right type of memory to use.
    i32 VulkanBuffer::get_memory_type_index(const VkMemoryRequirements& memory_requirements, VkMemoryPropertyFlags required_memory_properties) const {
        const VulkanDevice& device = *config.device;
        const VulkanPhysicalDevice& physical_device = device.get_physical_device();

        VkPhysicalDeviceMemoryProperties physical_device_memory_properties = physical_device.get_memory_properties();

        // The VkPhysicalDeviceMemoryProperties structure has two arrays memoryTypes and memoryHeaps. Memory heaps are distinct memory
        // resources like dedicated VRAM and swap space in RAM for when VRAM runs out. The different types of memory exist within these heaps.
        for (u32 memory_type_index = 0; memory_type_index < physical_device_memory_properties.memoryTypeCount; memory_type_index++) {

            // Check if the memory type is suitable for the buffer by checking if the corresponding bit is set to 1.
            bool memory_type_is_suitable = (memory_requirements.memoryTypeBits & 1 << memory_type_index) > 0;
            if (!memory_type_is_suitable) {
                continue;
            }

            // Check if the memory type has all required properties by doing a bitwise AND between the candidate and required properties,
            // and checking if the result is not just non-zero, but equal to the required properties bit field.
            //
            // The memoryTypes array consists of VkMemoryType structs that specify the heap and properties of each type of memory.
            // The properties define special features of the memory, like being able to map it so we can write to it from the CPU.
            //
            const VkMemoryType& memory_type = physical_device_memory_properties.memoryTypes[memory_type_index];
            bool memory_type_has_required_properties = (memory_type.propertyFlags & required_memory_properties) == required_memory_properties;
            if (!memory_type_has_required_properties) {
                continue;
            }

            return memory_type_index;
        }
        return -1;
    }
}
