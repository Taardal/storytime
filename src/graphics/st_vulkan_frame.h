#pragma once

#include "graphics/st_vulkan_uniform_buffer.h"
#include "graphics/st_vulkan_queue.h"
#include "graphics/st_vulkan_descriptor_set.h"
#include "graphics/st_vulkan_command_buffer.h"

namespace Storytime {
    struct Frame {
        VulkanQueue graphics_queue = nullptr;
        VulkanQueue present_queue = nullptr;
        VulkanDescriptorSet descriptor_set = nullptr;
        VulkanCommandBuffer command_buffer = nullptr;
        VulkanUniformBuffer* uniform_buffer = nullptr;
        VkFence in_flight_fence = nullptr;
        VkSemaphore image_available_semaphore = nullptr;
        VkSemaphore render_finished_semaphore = nullptr;
    };
}
