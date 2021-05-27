#include "SubTexture.h"

namespace storytime
{
    SubTexture::SubTexture(const SubTexture::Config& config)
            : texture(config.Texture), textureCoordinates{}, size(config.Size)
    {
        float minX = (config.Coordinates.x * config.Size.x) / (float) texture->GetWidth();
        float minY = (config.Coordinates.y * config.Size.y) / (float) texture->GetHeight();
        float maxX = ((config.Coordinates.x + 1) * config.Size.x) / (float) texture->GetWidth();
        float maxY = ((config.Coordinates.y + 1) * config.Size.y) / (float) texture->GetHeight();
        textureCoordinates[0] = { minX, minY };
        textureCoordinates[1] = { maxX, minY };
        textureCoordinates[2] = { maxX, maxY };
        textureCoordinates[3] = { minX, maxY };
    }

    const Ref<Texture>& SubTexture::GetTexture() const
    {
        return texture;
    }

    const glm::vec2* SubTexture::GetTextureCoordinates() const
    {
        return textureCoordinates;
    }

    const glm::vec2& SubTexture::GetSize() const
    {
        return size;
    }

}