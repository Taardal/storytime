#pragma once

namespace Storytime {
    struct QuadVertex {
        glm::vec4 position = { 0.0f, 0.0f, 0.0f, 0.0f };
        glm::vec2 texture_coordinate = { 0.0f, 0.0f };
    };

    struct QuadInstanceData {
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec4 texture_rectangle = { 0.0f, 0.0f, 0.0f, 0.0f };
        f32 texture_index = 0.0f;
    };
}
