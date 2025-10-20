#pragma once

#include "graphics/st_texture.h"

namespace Storytime {

    struct Quad {
        /// The model matrix of the quad that make up its transform in world space.
        glm::mat4 model = glm::mat4(1.0f);

        /// The color of the quad. If a texture is supplied, the color will be blended with the texture.
        glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        /// The texture to sample from in the fragment shader.
        Shared<Texture> texture = nullptr;

        /// Normalized coordinates for a rectangular area on the texture to sample from. Defaults to the entire texture.
        glm::vec4 texture_rectangle = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };
    };
}
