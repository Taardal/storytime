#include "st_sprite.h"

namespace Storytime {
    Sprite::Sprite(const SpriteConfig& config) : config(config) {
        ST_ASSERT(config.spritesheet_texture != nullptr, "Sprite must have a texture");
    }

    bool Sprite::is_valid() const {
        return config.spritesheet_texture != nullptr && config.width > 0 && config.height > 0;
    }

    void Sprite::invalidate() {
        config.spritesheet_texture = nullptr;
        config.width = 0;
        config.height = 0;
    }

    const Shared<Texture>& Sprite::get_texture() {
        return config.spritesheet_texture;
    }

    const glm::vec4& Sprite::get_texture_rectangle() const {
        return config.spritesheet_texture_rectangle;
    }

    u32 Sprite::get_width() const {
        return config.width;
    }

    u32 Sprite::get_height() const {
        return config.height;
    }

    Size<u32> Sprite::get_size() const {
        return { config.width, config.height };
    }
}
