#pragma once

namespace Storytime {
    struct QuadVertex {
        /// The position of the vertex in local space (relative to origin).
        glm::vec4 position = { 0.0f, 0.0f, 0.0f, 0.0f };

        /// The coordinate (UV) on a texture to sample from.
        glm::vec2 texture_coordinate = { 0.0f, 0.0f };
    };

    struct QuadInstanceData {
        /// The model matrix of the quad that make up its transform in world space.
        glm::mat4 model = glm::mat4(1.0f);

        /// The color of the quad. If a texture is supplied, the color will be blended with the texture.
        glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        /// Normalized coordinates for a rectangular area on the texture to sample from. Defaults to the entire texture.
        glm::vec4 texture_rectangle = { 0.0f, 0.0f, 1.0f, 1.0f };

        /// The index of the texture to sample from.
        f32 texture_index = 0.0f;
    };
}
