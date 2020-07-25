#include "system/Log.h"
#include "Texture.h"
#include <glad/glad.h>

namespace storytime
{
    constexpr int32_t Texture::TARGET = GL_TEXTURE_2D;
    constexpr int32_t Texture::LEVEL_OF_DETAIL = 0;
    constexpr int32_t Texture::BORDER = 0;

    Texture::Texture(const Image& image)
            : id(0),
              width(image.Width),
              height(image.Height),
              format(GetFormat(image.Channels)),
              internalFormat(GetInternalFormat(image.Channels))
    {
        CreateTexture();
        SetPixels(image.Pixels);
    }

    Texture::Texture(uint32_t width, uint32_t height)
            : id(0),
              width(width),
              height(height),
              format(GetFormat()),
              internalFormat(GetInternalFormat())
    {
        CreateTexture();
    }

    Texture::~Texture()
    {
        ST_GL_CALL(ST_TAG, glDeleteTextures(1, &id));
    }

    uint32_t Texture::GetId() const {
        return id;
    }

    void Texture::SetPixels(const void* pixels) const
    {
        ST_GL_CALL(ST_TAG, glTexImage2D(
                TARGET,
                LEVEL_OF_DETAIL,
                internalFormat,
                width,
                height,
                BORDER,
                format,
                GL_UNSIGNED_BYTE,
                pixels
        ));
    }

    void Texture::Bind(uint32_t textureSlot) const
    {
        ST_GL_CALL(ST_TAG, glActiveTexture(GL_TEXTURE0 + textureSlot));
        ST_GL_CALL(ST_TAG, glBindTexture(TARGET, id));
    }

    void Texture::Unbind() const
    {
        ST_GL_CALL(ST_TAG, glBindTexture(TARGET, 0));
    }

    void Texture::CreateTexture()
    {
        ST_GL_CALL(ST_TAG, glGenTextures(1, &id));
        ST_GL_CALL(ST_TAG, glBindTexture(TARGET, id));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_REPEAT));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, GL_REPEAT));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    }

    int32_t Texture::GetFormat(int32_t channels)
    {
        if (channels == 3)
        {
            return GL_RGB;
        }
        else
        {
            return GL_RGBA;
        }
    }

    int32_t Texture::GetInternalFormat(int32_t channels)
    {
        if (channels == 3)
        {
            return GL_RGB8;
        }
        else
        {
            return GL_RGBA8;
        }
    }

}

