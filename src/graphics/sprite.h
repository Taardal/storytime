#pragma once

#include "graphics/texture.h"
#include "graphics/renderer.h"

namespace Storytime {
    struct SpritesheetCoordinate {
        u32 row = 0;
        u32 column = 0;
    };

    struct SpriteConfig {
        Shared<Texture> spritesheet_texture = nullptr;
        u32 width = 0;
        u32 height = 0;
        std::vector<SpritesheetCoordinate> spritesheet_coordinates;
        std::vector<u32> animation_frame_durations_ms;
    };

    class Sprite {
    private:
        SpriteConfig config;
        std::vector<std::array<TextureCoordinate, 4>> texture_coordinate_list;
        u32 frame = 0;
        f64 frame_time_sec = 0.0;

    public:
        explicit Sprite(const SpriteConfig& config);

        const SpriteConfig& get_config() const;

        const SpritesheetCoordinate& get_spritesheet_coordinate(u32 frame = 0) const;

        void set_spritesheet_coordinates(const std::vector<SpritesheetCoordinate>& spritesheet_coordinates);

        void set_animation_frame_durations_ms(const std::vector<u32>& animation_frame_durations_ms);

        bool is_animated() const;

        void update(f64 timestep);

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
}
