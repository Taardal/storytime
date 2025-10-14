#pragma once

namespace Storytime {
    struct QuadVertex {
        glm::vec4 position = { 0.0f, 0.0f, 0.0f, 0.0f };
        glm::vec2 texture_coordinate = { 0.0f, 0.0f };

        static VkVertexInputBindingDescription getBindingDescription();

        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    struct InstanceData {
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    };
}