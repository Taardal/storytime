#include "spritesheet.h"

namespace Storytime {
    Spritesheet::Spritesheet(const SpritesheetConfig& config) : config(config) {
    }

    Sprite Spritesheet::get_sprite(SpriteConfig& sprite_config) const {
        sprite_config.spritesheet_texture = config.texture;
        return Sprite(sprite_config);
    }

    Shared<Texture> Spritesheet::get_texture() const {
        return config.texture;
    }
}
