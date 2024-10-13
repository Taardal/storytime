#include "sprite.h"

namespace Storytime {
    Sprite::Sprite(const SpriteConfig& config) : config(config) {
        if (config.spritesheet_coordinates.size() > 0) {
            set_spritesheet_coordinates(config.spritesheet_coordinates);
        } else {
            set_spritesheet_coordinates({ config.spritesheet_coordinate });
        }
    }

    const SpriteConfig& Sprite::get_config() const {
        return config;
    }

    const SpritesheetCoordinate& Sprite::get_spritesheet_coordinate(u32 frame) const {
        ST_ASSERT(frame < spritesheet_coordinates.size(), "Sprite frame index out of bounds: [" << frame << "] < [" << spritesheet_coordinates.size() << "]");
        return spritesheet_coordinates[frame];
    }

    void Sprite::set_spritesheet_coordinates(const std::vector<SpritesheetCoordinate>& spritesheet_coordinates) {
        ST_ASSERT(spritesheet_coordinates.size() > 0, "Sprite must have at least one spritesheet coordinate (frame) to be rendered");
        this->spritesheet_coordinates = spritesheet_coordinates;

        // Clear textures coordinates if they have already been set
        if (spritesheet_texture_coordinates.size() > 0) {
            spritesheet_texture_coordinates = std::vector<std::array<TextureCoordinate, 4>>();
        }

        f32 sprite_width = (f32) config.width;
        f32 sprite_height = (f32) config.height;

        f32 spritesheet_width = (f32) config.spritesheet_texture->get_width();
        f32 spritesheet_height = (f32) config.spritesheet_texture->get_height();

        for (auto& spritesheet_coordinate : spritesheet_coordinates) {
            f32 x = spritesheet_coordinate.column * sprite_width;
            f32 y = spritesheet_coordinate.row * sprite_height;

            f32 x_left = x / spritesheet_width;
            f32 x_right = (x + sprite_width) / spritesheet_width;

            f32 y_top = y / spritesheet_height;
            f32 y_bottom = (y + sprite_height) / spritesheet_height;

            std::array<TextureCoordinate, 4> texture_coordinates;
            texture_coordinates[0] = {x_left, y_top};     // Top left
            texture_coordinates[1] = {x_right, y_top};    // Top right
            texture_coordinates[2] = {x_right, y_bottom}; // Bottom right
            texture_coordinates[3] = {x_left, y_bottom};  // Bottom left

            spritesheet_texture_coordinates.push_back(texture_coordinates);
        }
    }

    bool Sprite::is_animated() const {
        return spritesheet_coordinates.size() > 1;
    }

    void Sprite::update(f64 timestep) {
        ST_ASSERT(frame < spritesheet_coordinates.size(), "Sprite frame index out of bounds: [" << frame << "] < [" << spritesheet_coordinates.size() << "]");
        u32 frame_duration_ms = spritesheet_coordinates[frame].frame_duration_ms;

        frame_time_sec += timestep;
        if (frame_time_sec * 1000 < frame_duration_ms) {
            return;
        }

        frame = (frame + 1) % spritesheet_coordinates.size();
        frame_time_sec = 0.0;
    }

    void Sprite::render(Renderer& renderer, const RenderConfig& render_config) const {
        f32 x = render_config.position.x;
        f32 y = render_config.position.y;

        f32 scale = render_config.scale;

        f32 quad_x = x * scale;
        f32 quad_y = y * scale;

        u32 sprite_width = config.width;
        u32 sprite_height = config.height;

        f32 quad_width = sprite_width * scale;
        f32 quad_height = sprite_height * scale;

        if (render_config.debug) {
            Quad debug_quad;
            debug_quad.color = render_config.debug_color;
            debug_quad.size = { quad_width, quad_height };
            debug_quad.position = { quad_x, quad_y, 0.0f };
            renderer.render_quad(debug_quad);
        }

        Quad quad;
        quad.texture = config.spritesheet_texture;
        quad.size = { quad_width, quad_height };
        quad.position = { quad_x, quad_y, 0.0f };
        renderer.render_quad(quad, spritesheet_texture_coordinates[frame]);
    }
}