#pragma once

#include "graphics/texture.h"
#include "graphics/renderer.h"

namespace Storytime {
    struct SpriteConfig {
        Shared<Texture> spritesheet_texture = nullptr;
        u32 width = 0;
        u32 height = 0;
        u32 row = 0;
        u32 column = 0;
        u32 start_row = 0;
        u32 end_row = 0;
        u32 start_column = 0;
        u32 end_column = 0;
    };
    class Sprite {
    private:
        SpriteConfig config;
        std::vector<std::array<TextureCoordinate, 4>> texture_coordinate_list;

    public:
        explicit Sprite(const SpriteConfig& config);

        struct RenderConfig {
            glm::vec2 position = { 0.0f, 0.0f };
            f32 scale = 1.0f;
            bool debug = false;
        };

        void render(Renderer& renderer, const RenderConfig& render_config) const;
    };

    struct SpritesheetConfig {
        Shared<Texture> texture = nullptr;
    };

    class Spritesheet {
    private:
        SpritesheetConfig config;

    public:
        explicit Spritesheet(SpritesheetConfig config);

        Sprite get_sprite(SpriteConfig& sprite_config) const;
    };
}
