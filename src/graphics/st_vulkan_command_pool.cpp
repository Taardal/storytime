#include "st_vulkan_command_pool.h"

namespace Storytime {
    VulkanCommandPool::VulkanCommandPool(const Config& config) : config(config) {
        create_command_pool();
    }

    VulkanCommandPool::~VulkanCommandPool() {
        destroy_command_pool();
    }

    void VulkanCommandPool::allocate_command_buffers(u32 command_buffer_count, VkCommandBuffer* command_buffers) const {
        VkCommandBufferAllocateInfo command_buffer_allocate_info{};
        command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_allocate_info.commandPool = command_pool;
        command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        command_buffer_allocate_info.commandBufferCount = command_buffer_count;

        if (config.device->allocate_command_buffers(command_buffer_allocate_info, command_buffers) != VK_SUCCESS) {
            ST_THROW("Could not allocate Vulkan command buffers");
        }
    }

    std::vector<VkCommandBuffer> VulkanCommandPool::allocate_command_buffers(u32 command_buffer_count) const {
        std::vector<VkCommandBuffer> command_buffers(command_buffer_count);
        allocate_command_buffers(command_buffers.size(), command_buffers.data());
        return command_buffers;
    }

    void VulkanCommandPool::create_command_pool() {
        const QueueFamilyIndices& queue_family_indices = config.physical_device->get_queue_family_indices();

        VkCommandPoolCreateInfo command_pool_create_info{};
        command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        command_pool_create_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

        if (config.device->create_command_pool(command_pool_create_info, &command_pool) != VK_SUCCESS) {
            ST_THROW("Could not create Vulkan command pool");
        }

        if (config.device->set_object_name(command_pool, VK_OBJECT_TYPE_COMMAND_POOL, config.name.c_str())) {
            ST_THROW("Could not set Vulkan command pool name [" << config.name << "]");
        }
    }

    void VulkanCommandPool::destroy_command_pool() const {
        config.device->destroy_command_pool(command_pool);
    }
}
