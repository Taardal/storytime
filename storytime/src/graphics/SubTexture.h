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
            Ref<Texture> texture;
            glm::vec2 coordinates;
            glm::vec2 size;
        };

    private:
        Ref<Texture> texture;
        glm::vec2 textureCoordinates[4];

    public:
        explicit SubTexture(const Config& config);

        SubTexture(const glm::vec2& min, const glm::vec2& max, const Ref<Texture>& texture);

        [[nodiscard]] const Ref<storytime::Texture>& GetTexture() const;

        [[nodiscard]] const glm::vec2* GetTextureCoordinates() const;

        void Init(const glm::vec2& min, const glm::vec2& max);
    };
}


