#pragma once

#include "graphics/texture.h"
#include "graphics/renderer.h"

namespace Storytime {
    struct SpriteConfig {
        Shared<Texture> texture = nullptr;
        u32 width = 0;
        u32 height = 0;
    };

    class Sprite {
    private:
        SpriteConfig config;

    public:
        explicit Sprite(SpriteConfig config);
    };

    struct SpritesheetConfig {
        Shared<Texture> texture = nullptr;
        u32 sprite_width = 0;
        u32 sprite_height = 0;
    };

    class Spritesheet {
    private:
        SpritesheetConfig config;

    public:
        explicit Spritesheet(SpritesheetConfig config);

        const SpritesheetConfig& get_config() const;

        Sprite get_sprite() const;

        void set_sprite_width(u32 sprite_width);

        void set_sprite_height(u32 sprite_height);

        void set_sprite_size(u32 sprite_size);

        void render(Renderer* renderer, const glm::vec3& position, const glm::vec2& frame, const glm::vec2& size) const;
    };
}
