#pragma once

#include "Texture.h"
#include "system/Core.h"
#include <glm/glm.hpp>

namespace storytime
{
    class SubTexture
    {
    public:
        struct Config
        {
            Ref<Texture> Texture;
            glm::vec2 Coordinates;
            glm::vec2 Size;
        };

    private:
        Ref<Texture> texture;
        glm::vec2 textureCoordinates[4];
        glm::vec2 size;

    public:
        explicit SubTexture(const Config& config);

        [[nodiscard]] const Ref<storytime::Texture>& GetTexture() const;

        [[nodiscard]] const glm::vec2* GetTextureCoordinates() const;

        [[nodiscard]] const glm::vec2& GetSize() const;
    };
}


