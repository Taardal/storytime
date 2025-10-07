#include "st_vulkan_command_pool.h"

#include "graphics/st_vulkan_command_buffer.h"

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
            ST_THROW("Could not allocate [" << command_buffer_count << "] command buffers");
        }
    }

    std::vector<VkCommandBuffer> VulkanCommandPool::allocate_command_buffers(u32 command_buffer_count) const {
        std::vector<VkCommandBuffer> command_buffers(command_buffer_count);
        allocate_command_buffers(command_buffers.size(), command_buffers.data());
        return command_buffers;
    }

    void VulkanCommandPool::allocate_command_buffer(VkCommandBuffer* command_buffer) const {
        allocate_command_buffers(1, command_buffer);
    }

    VkCommandBuffer VulkanCommandPool::allocate_command_buffer() const {
        VkCommandBuffer command_buffer = nullptr;
        allocate_command_buffers(1, &command_buffer);
        return command_buffer;
    }

    void VulkanCommandPool::free_command_buffers(u32 command_buffer_count, const VkCommandBuffer* command_buffers) const {
        config.device->free_command_buffers(command_pool, command_buffer_count, command_buffers);
    }

    void VulkanCommandPool::free_command_buffer(VkCommandBuffer command_buffer) const {
        free_command_buffers(1, &command_buffer);
    }

    void VulkanCommandPool::with_command_buffer(const WithCommandBufferFn& with_command_buffer) const {
        VulkanCommandBuffer command_buffer = allocate_command_buffer();
        with_command_buffer(command_buffer);
        free_command_buffer(command_buffer);
    }

    VkCommandBuffer VulkanCommandPool::begin_one_time_submit_command_buffer() const {
        VulkanCommandBuffer command_buffer = allocate_command_buffer();
        command_buffer.begin_one_time_submit();
        return command_buffer;
    }

    void VulkanCommandPool::end_one_time_submit_command_buffer(VkCommandBuffer command_buffer) const {
        const VulkanDevice& device = *config.device;
        VkQueue queue = device.get_queue(config.queue_family_index);
        VulkanCommandBuffer(command_buffer).end_one_time_submit(queue, device);
        free_command_buffer(command_buffer);
    }

    void VulkanCommandPool::record_and_submit_commands(const RecordCommandsFn& record_commands) const {
        VulkanCommandBuffer command_buffer = begin_one_time_submit_command_buffer();
        record_commands(command_buffer);
        end_one_time_submit_command_buffer(command_buffer);
    }

    void VulkanCommandPool::record_and_submit_commands(const RecordAndSubmitCommandsFn& record_and_submit_commands) const {
        record_and_submit_commands([&](const RecordCommandsFn& record_commands) {
            VulkanCommandBuffer command_buffer = begin_one_time_submit_command_buffer();
            record_commands(command_buffer);
            end_one_time_submit_command_buffer(command_buffer);
        });
    }

    void VulkanCommandPool::create_command_pool() {
        VkCommandPoolCreateInfo command_pool_create_info{};
        command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        command_pool_create_info.flags = config.flags;
        command_pool_create_info.queueFamilyIndex = config.queue_family_index;

        if (config.device->create_command_pool(command_pool_create_info, &command_pool) != VK_SUCCESS) {
            ST_THROW("Could not create Vulkan command pool");
        }

        if (config.device->set_object_name(command_pool, VK_OBJECT_TYPE_COMMAND_POOL, config.name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set Vulkan command pool name [" << config.name << "]");
        }
    }

    void VulkanCommandPool::destroy_command_pool() const {
        config.device->destroy_command_pool(command_pool);
    }
}
