#pragma once

namespace Storytime {
    struct Frame {
        VkCommandBuffer command_buffer = nullptr;
        VkFence in_flight_fence = nullptr;
        VkSemaphore image_available_semaphore = nullptr;
        VkSemaphore render_finished_semaphore = nullptr;
        VkQueue graphics_queue = nullptr;
        VkQueue present_queue = nullptr;
    };
}