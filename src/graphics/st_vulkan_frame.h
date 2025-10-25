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

        QuadInstanceData& get_current_quad() const {
            ST_ASSERT_IN_BOUNDS(quad_index, (*quads));
            return quads->at(quad_index);
        }

        std::shared_ptr<VulkanImage>& get_current_texture() const {
            ST_ASSERT_IN_BOUNDS(texture_index, (*textures));
            return textures->at(texture_index);
        }

        bool is_full() const {
            ST_ASSERT_IN_BOUNDS(quad_index, (*quads));
            if (quad_index == quads->size() - 1) {
                return true;
            }
            ST_ASSERT_IN_BOUNDS(texture_index, (*textures));
            if (texture_index == textures->size() - 1) {
                return true;
            }
            return false;
        }
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

        Batch& get_current_batch() {
            ST_ASSERT_IN_BOUNDS(batch_index, batches);
            return batches.at(batch_index);
        }
    };
}
