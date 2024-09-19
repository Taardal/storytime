#include "spritesheet.h"

namespace Storytime {
    Spritesheet::Spritesheet(SpritesheetConfig config) : config(std::move(config)) {
    }

    Sprite Spritesheet::get_sprite(SpriteConfig& sprite_config) const {
        sprite_config.spritesheet_texture = config.texture;
        return Sprite(sprite_config);
    }
}
