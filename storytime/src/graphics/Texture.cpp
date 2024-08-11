#include "system/log.h"
#include "Texture.h"
#include "system/Tag.h"
#include "graphics/GraphicsLog.h"

#include <glad/glad.h>

namespace Storytime
{
    Texture::Config::Config()
        : Width(0), Height(0), Format(0), InternalFormat(0), LevelOfDetail(0), Border(0)
    {}
}

namespace Storytime
{
    constexpr int32_t Texture::TARGET = GL_TEXTURE_2D;

    Texture::Texture(const Texture::Config& config)
            : id(0),
              width(config.Width),
              height(config.Height),
              format(config.Format),
              internalFormat(config.InternalFormat),
              levelOfDetail(config.LevelOfDetail),
              border(config.Border)
    {
        Init();
        SetPixels(nullptr);
    }

    Texture::Texture(const Image& image)
            : id(0),
              width(image.Width),
              height(image.Height),
              format(image.Channels == 3 ? GL_RGB : GL_RGBA),
              internalFormat(image.Channels == 3 ? GL_RGB8 : GL_RGBA8),
              levelOfDetail(0),
              border(0)
    {
        Init();
        SetPixels(image.Pixels);
    }

    Texture::Texture(uint32_t width, uint32_t height)
            : id(0),
              width(width),
              height(height),
              format(GL_RGBA),
              internalFormat(GL_RGBA8),
              levelOfDetail(0),
              border(0)
    {
        Init();
        SetPixels(nullptr);
    }

    Texture::~Texture()
    {
        ST_GL_CALL(ST_TAG, glDeleteTextures(1, &id));
    }

    uint32_t Texture::GetId() const
    {
        return id;
    }

    int32_t Texture::GetLevelOfDetail() const
    {
        return levelOfDetail;
    }

    void Texture::SetPixels(const void* pixels) const
    {
        ST_GL_CALL(ST_TAG, glTexImage2D(
                TARGET,
                levelOfDetail,
                internalFormat,
                width,
                height,
                border,
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

    void Texture::Init()
    {
        ST_GL_CALL(ST_TAG, glGenTextures(1, &id));
        ST_GL_CALL(ST_TAG, glBindTexture(TARGET, id));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_REPEAT));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, GL_REPEAT));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    }

}

