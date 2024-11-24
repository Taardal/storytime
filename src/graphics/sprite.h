#pragma once

#include "graphics/texture.h"
#include "graphics/renderer.h"

namespace Storytime {
    struct SpriteCollider {
        f32 x;
        f32 y;
        f32 width;
        f32 height;
    };

    struct SpritesheetCoordinate {
        u32 row = 0;
        u32 column = 0;
        u32 frame_duration_ms = 0; // Animated sprite
    };

    struct SpriteConfig {
        Shared<Texture> spritesheet_texture = nullptr;
        u32 width = 0;
        u32 height = 0;
        std::vector<SpritesheetCoordinate> spritesheet_coordinates;
    };

    class Sprite {
    private:
        SpriteConfig config;
        std::vector<SpritesheetCoordinate> spritesheet_coordinates;
        std::vector<std::array<TextureCoordinate, 4>> spritesheet_texture_coordinates;
        u32 frame = 0;
        f64 frame_time_sec = 0.0;

    public:
        Sprite() = default;

        explicit Sprite(const SpriteConfig& config);

        const SpriteConfig& get_config() const;

        const SpritesheetCoordinate& get_spritesheet_coordinate(u32 frame = 0) const;

        void set_spritesheet_coordinates(const std::vector<SpritesheetCoordinate>& spritesheet_coordinates);

        bool is_animated() const;

        void update(f64 timestep);

        struct RenderConfig {
            glm::vec2 position = { 0.0f, 0.0f };
            f32 scale = 1.0f;
            bool debug = false;
            glm::vec4 debug_color = { 1.0f, 0.0f, 1.0f, 1.0f };
            glm::vec4 debug_collider_color = { 0.0f, 1.0f, 1.0f, 1.0f };
            glm::mat4 rotation = glm::mat4(1.0f);
            f32 rotation_deg = 0.0f;
            bool flip_horizontally = false;
            bool flip_vertically = false;
            SpriteCollider collider;
        };

        void render(Renderer* renderer, const RenderConfig& render_config) const;
    };

    struct SpritesheetConfig {
        Shared<Texture> texture = nullptr;
    };
}
