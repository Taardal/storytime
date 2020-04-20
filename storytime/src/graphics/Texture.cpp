#include "system/Log.h"
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
        ST_GL_CALL(ST_TAG, glDeleteTextures(1, &id));
    }

    void Texture::SetPixels(const void* pixels) const
    {
        ST_GL_CALL(ST_TAG, glTexImage2D(
                TARGET,
                LEVEL_OF_DETAIL,
                GL_RGB,
                width,
                height,
                BORDER,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                pixels
        ));
    }

    void Texture::Bind(uint32_t textureUnit) const
    {
        ST_GL_CALL(ST_TAG, glActiveTexture(GL_TEXTURE0 + textureUnit));
        ST_GL_CALL(ST_TAG, glBindTexture(TARGET, id));
    }

    void Texture::Unbind() const
    {
        ST_GL_CALL(ST_TAG, glBindTexture(TARGET, 0));
    }

    void Texture::Init()
    {
        ST_GL_CALL(ST_TAG, glGenTextures(1, &id));
        ST_GL_CALL(ST_TAG, glBindTexture(TARGET, id));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_REPEAT));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, GL_REPEAT));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }

}

