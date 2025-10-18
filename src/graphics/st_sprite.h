#pragma once

/// A @Sprite represents a single square in the image grid of a @Spritesheet

#include "st_renderer.h"
#include "st_texture.h"
#include "st_texture_coordinate.h"

namespace Storytime {
    struct SpriteRenderConfig {
        glm::vec2 position = { 0.0f, 0.0f };
        f32 scale = 1.0f;
        f32 rotation_deg = 0.0f;
        bool flipped_horizontally = false;
        bool flipped_vertically = false;
        bool flipped_diagonally = false;
    };

    struct SpriteConfig {
        Shared<Texture> spritesheet_texture = nullptr;
        glm::vec4 spritesheet_texture_rectangle{};
        u32 spritesheet_row = 0;
        u32 spritesheet_column = 0;
        u32 width = 0;
        u32 height = 0;
    };

    class Sprite {
    private:
        SpriteConfig config;

    public:
        Sprite() = default;

        Sprite(const SpriteConfig& config);

        bool is_valid() const;

        void invalidate();

        Shared<Texture> get_texture();

        u32 get_width() const;

        u32 get_height() const;

        Size<u32> get_size() const;

        void render(Renderer* renderer, const SpriteRenderConfig& render_config) const;
    };
}
