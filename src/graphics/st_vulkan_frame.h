#pragma once

#include "graphics/st_quad_vertex.h"
#include "graphics/st_vulkan_command_buffer.h"
#include "graphics/st_vulkan_descriptor_set.h"
#include "graphics/st_vulkan_image.h"
#include "graphics/st_vulkan_queue.h"
#include "graphics/st_vulkan_uniform_buffer.h"
#include "graphics/st_vulkan_vertex_buffer.h"

namespace Storytime {
    struct Batch {
        VulkanInstanceBuffer* vertex_buffer = nullptr;
        VulkanDescriptorSet descriptor_set = nullptr;
        std::vector<QuadInstanceData>* quads = nullptr;
        u32 quad_index = 0;
        std::vector<std::shared_ptr<VulkanImage>>* textures = nullptr;
        u32 texture_index = 0;
    };

    struct Frame {
        VulkanQueue graphics_queue = nullptr;
        VulkanQueue present_queue = nullptr;
        VulkanDescriptorSet descriptor_set = nullptr;
        VulkanCommandBuffer command_buffer = nullptr;
        VulkanUniformBuffer* uniform_buffer = nullptr;
        VkFence in_flight_fence = nullptr;
        VkSemaphore image_available_semaphore = nullptr;
        VkSemaphore render_finished_semaphore = nullptr;
        std::vector<Batch> batches{};
        u32 batch_index = 0;

        Batch& get_batch() {
            ST_ASSERT(
                batch_index < batches.size(),
                "Batch index [" << batch_index << "] must be less than batch count [" << batches.size() << "]"
            );
            return batches.at(batch_index);
        }
    };
}
