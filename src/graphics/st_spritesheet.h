#pragma once

/// A @Spritesheet represents an image that contain a set of @Sprite organized in a grid.
/// A row of the spritesheet may make up a sequence of sprites to form an animation.

#include "st_texture.h"

namespace Storytime {

    // Forward declarations
    struct Sprite;
    struct Animation;

    struct SpritesheetConfig {
        Shared<Texture> texture = nullptr;
    };

    class Spritesheet {
    private:
        SpritesheetConfig config;

    public:
        explicit Spritesheet(const SpritesheetConfig& config);

        Shared<Texture> get_texture() const;

        struct SpriteCoordinate {
            u32 row;
            u32 column;
            u32 width;
            u32 height;
        };

        Sprite get_sprite(u32 row, u32 column, u32 width, u32 height) const;

        struct AnimationCoordinate {
            u32 row;
            u32 column_count;
            u32 sprite_width;
            u32 sprite_height;
        };

        Animation get_animation(u32 row, u32 column_count, u32 sprite_width, u32 sprite_height) const;
    };
}
