#include "SubTexture.h"

namespace storytime
{
    SubTexture::SubTexture(const SubTexture::Config& config)
            : texture(config.texture), textureCoordinates{}
    {
        float minX = (config.coordinates.x * config.size.x) / texture->GetWidth();
        float minY = (config.coordinates.y * config.size.y) / texture->GetHeight();
        float maxX = ((config.coordinates.x + 1) * config.size.x) / texture->GetWidth();
        float maxY = ((config.coordinates.y + 1) * config.size.y) / texture->GetHeight();
        Init({ minX, minY }, { maxX, maxY });
    }

    SubTexture::SubTexture(const glm::vec2& min, const glm::vec2& max, const Ref<Texture>& texture)
            : texture(texture), textureCoordinates{}
    {
        Init(min, max);
    }

    void SubTexture::Init(const glm::vec2& min, const glm::vec2& max)
    {
        textureCoordinates[0] = { min.x, min.y };
        textureCoordinates[1] = { max.x, min.y };
        textureCoordinates[2] = { max.x, max.y };
        textureCoordinates[3] = { min.x, max.y };
    }

    const Ref<Texture>& SubTexture::GetTexture() const
    {
        return texture;
    }

    const glm::vec2* SubTexture::GetTextureCoordinates() const
    {
        return textureCoordinates;
    }

}