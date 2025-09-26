#include "st_texture.h"
#include "graphics/st_open_gl.h"

namespace Storytime {
    constexpr i32 Texture::TARGET = GL_TEXTURE_2D;

    Texture::Texture(const TextureConfig& config)
        : id(0),
          width(config.width),
          height(config.height),
          format(config.format),
          internal_format(config.internal_format),
          level_of_detail(config.level_of_detail),
          border(config.border)
    {
        initialize();
        set_pixels(nullptr);
    }

    Texture::Texture(const Image& image)
        : id(0),
          width(image.width),
          height(image.height),
          format(image.channels == 3 ? GL_RGB : GL_RGBA),
          internal_format(image.channels == 3 ? GL_RGB8 : GL_RGBA8),
          level_of_detail(0),
          border(0)
    {
        initialize();
        set_pixels(image.pixels);
    }

    Texture::Texture(u32 width, u32 height)
        : id(0),
          width(width),
          height(height),
          format(GL_RGBA),
          internal_format(GL_RGBA8),
          level_of_detail(0),
          border(0)
    {
        initialize();
        set_pixels(nullptr);
    }

    Texture::~Texture() {
        ST_GL_CALL(glDeleteTextures(1, &id));
    }

    std::pair<u32, u32> Texture::get_size() const {
        return { width, height };
    }

    u32 Texture::get_width() const {
        return width;
    }

    u32 Texture::get_height() const {
        return height;
    }

    u32 Texture::get_id() const {
        return id;
    }

    i32 Texture::get_level_of_detail() const {
        return level_of_detail;
    }

    void Texture::set_pixels(const void* pixels) const {
        ST_GL_CALL(glTexImage2D(
            TARGET,
            level_of_detail,
            internal_format,
            width,
            height,
            border,
            format,
            GL_UNSIGNED_BYTE,
            pixels
        ));
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::bind(u32 texture_slot) const {
        ST_GL_CALL(glActiveTexture(GL_TEXTURE0 + texture_slot));
        ST_GL_CALL(glBindTexture(TARGET, id));
    }

    void Texture::unbind() const {
        ST_GL_CALL(glBindTexture(TARGET, 0));
    }

    void Texture::initialize() {
        ST_GL_CALL(glGenTextures(1, &id));
        ST_GL_CALL(glBindTexture(TARGET, id));
        ST_GL_CALL(glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_REPEAT));
        ST_GL_CALL(glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, GL_REPEAT));
        ST_GL_CALL(glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
        ST_GL_CALL(glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    }
}
