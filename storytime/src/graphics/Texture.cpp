#include "Texture.h"
#include <glad/glad.h>

namespace storytime
{
    constexpr int32_t Texture::TARGET = GL_TEXTURE_2D;
    constexpr int32_t Texture::LEVEL_OF_DETAIL = 0;
    constexpr int32_t Texture::BORDER = 0;

    Texture::Texture(const Image& image)
            : id(0), width(image.Width), height(image.Height)
    {
        Init();
        SetPixels(image.Pixels);
    }

    Texture::Texture(uint32_t width, uint32_t height)
            : id(0), width(width), height(height)
    {
        Init();
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &id);
    }

    void Texture::SetPixels(const void* pixels) const
    {
        glTexImage2D(
                TARGET,
                LEVEL_OF_DETAIL,
                GL_RGB,
                width,
                height,
                BORDER,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                pixels
        );
    }

    void Texture::Bind(uint32_t textureUnit) const
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(TARGET, id);
    }

    void Texture::Unbind() const
    {
        glBindTexture(TARGET, 0);
    }

    void Texture::Init()
    {
        glGenTextures(1, &id);
        glBindTexture(TARGET, id);
        glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

}

