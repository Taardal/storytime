#pragma once

#include <storytime/storytime.h>

namespace Storytime {
    struct QuadVertex {
        glm::vec2 position = { 0.0f, 0.0f };
        glm::vec3 color = { 0.0f, 0.0f, 0.0f };

        static VkVertexInputBindingDescription getBindingDescription();

        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };
}