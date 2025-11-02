#pragma once

/// A @Sprite represents a single square in the image grid of a @Spritesheet

#include "st_texture.h"

namespace Storytime {
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

        const Shared<Texture>& get_texture();

        const glm::vec4& get_texture_rectangle() const;

        u32 get_width() const;

        u32 get_height() const;

        Size<u32> get_size() const;
    };
}
