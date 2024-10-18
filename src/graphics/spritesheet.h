#pragma once

#include "graphics/sprite.h"

namespace Storytime {
    class Spritesheet {
    private:
        SpritesheetConfig config;

    public:
        explicit Spritesheet(SpritesheetConfig config);

        Sprite get_sprite(SpriteConfig& sprite_config) const;

        Shared<Texture> get_texture() const;
    };
}
