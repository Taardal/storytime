#include "st_vulkan_command_pool.h"
#include "graphics/st_vulkan_command_buffer.h"
#include "graphics/st_vulkan_queue.h"

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

        ST_ASSERT_THROW_VK(
            config.device.allocate_command_buffers(command_buffer_allocate_info, command_buffers),
            "Could not allocate [" << command_buffer_count << "] command buffers"
        );
    }

    std::vector<VkCommandBuffer> VulkanCommandPool::allocate_command_buffers(u32 command_buffer_count) const {
        std::vector<VkCommandBuffer> command_buffers(command_buffer_count);
        allocate_command_buffers(command_buffers.size(), command_buffers.data());
        return command_buffers;
    }

    void VulkanCommandPool::allocate_command_buffer(VkCommandBuffer* command_buffer, std::string_view name) const {
        allocate_command_buffers(1, command_buffer);
        if (name.empty()) {
           return;
        }
        VkResult name_result = config.device.set_object_name(*command_buffer, VK_OBJECT_TYPE_COMMAND_BUFFER, name);
        if (name_result != VK_SUCCESS) {
            ST_LOG_E("Could not set command buffer name [{}]: {}", name, format_vk_result(name_result));
        }
    }

    VkCommandBuffer VulkanCommandPool::allocate_command_buffer(std::string_view name) const {
        VkCommandBuffer command_buffer = nullptr;
        allocate_command_buffer(&command_buffer, name);
        return command_buffer;
    }

    void VulkanCommandPool::free_command_buffers(u32 command_buffer_count, const VkCommandBuffer* command_buffers) const {
        config.device.free_command_buffers(command_pool, command_buffer_count, command_buffers);
    }

    void VulkanCommandPool::free_command_buffer(VkCommandBuffer command_buffer) const {
        free_command_buffers(1, &command_buffer);
    }

    VkCommandBuffer VulkanCommandPool::begin_one_time_submit_command_buffer(std::string_view name) const {
        VulkanCommandBuffer command_buffer = allocate_command_buffer(name);
        command_buffer.begin_one_time_submit();
        return command_buffer;
    }

    void VulkanCommandPool::end_one_time_submit_command_buffer(VkCommandBuffer command_buffer) const {
        VkQueue queue = config.device.get_queue(config.queue_family_index);
        VulkanCommandBuffer(command_buffer).end_one_time_submit(queue, config.device);
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

        ST_ASSERT_THROW_VK(
            config.device.create_command_pool(command_pool_create_info, &command_pool, config.name),
            "Could not create Vulkan command pool"
        );
    }

    void VulkanCommandPool::destroy_command_pool() const {
        config.device.destroy_command_pool(command_pool);
    }
}
