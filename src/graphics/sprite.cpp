#include "sprite.h"

namespace Storytime {
    Sprite::Sprite(const SpriteConfig& config) : config(config), size({ config.width, config.height }) {
        ST_ASSERT(config.spritesheet_texture != nullptr, "Sprite must have a texture");
        ST_ASSERT(config.spritesheet_coordinates.size() > 0, "Sprite must have at least one texture coordinate");
        set_spritesheet_coordinates(config.spritesheet_coordinates);
    }

    const SpriteConfig& Sprite::get_config() const {
        return config;
    }

    const SpritesheetCoordinate& Sprite::get_spritesheet_coordinate(u32 frame) const {
        ST_ASSERT(
            frame < spritesheet_coordinates.size(),
            "Sprite frame index out of bounds: [" << frame << "] < [" << spritesheet_coordinates.size() << "]"
        );
        return spritesheet_coordinates[frame];
    }

    void Sprite::set_spritesheet_coordinates(const std::vector<SpritesheetCoordinate>& spritesheet_coordinates) {
        ST_ASSERT(
            spritesheet_coordinates.size() > 0,
            "Sprite must have at least one spritesheet coordinate (frame) to be rendered"
        );
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

            spritesheet_texture_coordinates.emplace_back(texture_coordinates);
        }
    }

    bool Sprite::is_animated() const {
        return spritesheet_coordinates.size() > 1;
    }

    const Size<u32>& Sprite::get_size() const {
        return size;
    }

    void Sprite::update(f64 timestep) {
        ST_ASSERT(
            frame < spritesheet_coordinates.size(),
            "Sprite frame index out of bounds: [" << frame << "] < [" << spritesheet_coordinates.size() << "]"
        );
        u32 frame_duration_ms = spritesheet_coordinates[frame].frame_duration_ms;

        frame_time_sec += timestep;
        if (frame_time_sec * 1000 < frame_duration_ms) {
            return;
        }

        frame = (frame + 1) % spritesheet_coordinates.size();
        frame_time_sec = 0.0;
    }

    void Sprite::render(Renderer* renderer, const SpriteRenderConfig& render_config) const {
        ST_ASSERT(config.spritesheet_texture != nullptr, "Sprite must have a texture to be rendered");

        f32 sprite_width = (f32) config.width;
        f32 sprite_height = (f32) config.height;

        f32 sprite_x = render_config.position.x;
        f32 sprite_y = render_config.position.y;

        if (render_config.debug) {
            Quad sprite_debug_quad = {
                .color = render_config.debug_color,
                .size = { sprite_width, sprite_height },
                .position = { sprite_x, sprite_y, 0.0f },
            };
            renderer->render_quad(sprite_debug_quad);

            f32 collider_x = render_config.collider.x;
            f32 collider_y = render_config.collider.y;

            f32 collider_width = render_config.collider.width;
            f32 collider_height = render_config.collider.height;

            f32 collider_quad_x = sprite_x - sprite_width / 2 + collider_width / 2;
            f32 collider_quad_y = sprite_y - sprite_height / 2 + collider_height / 2;

            if (render_config.flip_horizontally) {
                collider_quad_x += sprite_width - (collider_x + collider_width);
            } else {
                collider_quad_x += collider_x;
            }
            if (render_config.flip_vertically) {
                collider_quad_y += sprite_height - (collider_y + collider_height);
            } else {
                collider_quad_y += collider_y;
            }

            Quad collider_debug_quad = {
                .color = render_config.debug_collider_color,
                .size = { collider_width, collider_height },
                .position = { collider_quad_x, collider_quad_y, 0.0f },
                .rotation_in_degrees = render_config.rotation_deg,
            };
            renderer->render_quad(collider_debug_quad);
        }

        Quad quad = {
            .texture = config.spritesheet_texture,
            .size = { sprite_width, sprite_height },
            .position = { sprite_x, sprite_y, 0.0f },
            .rotation_in_degrees = render_config.rotation_deg,
        };

        bool flipped = render_config.flip_horizontally || render_config.flip_vertically || render_config.flip_diagonally;
        if (!flipped) {
            renderer->render_quad(quad, spritesheet_texture_coordinates[frame]);
            return;
        }

        // Sprite should be flipped
        // Copy the texture coordinates to preserve the normal/unflipped ones
        auto texture_coordinates = spritesheet_texture_coordinates[frame];

        // Switch left and right texture coordinates to flip sprite horizontally
        if (render_config.flip_horizontally) {
            f32 x_left = texture_coordinates[0].x;   // Top left
            f32 x_right = texture_coordinates[1].x;  // Top right
            texture_coordinates[0].x = x_right;      // Left --> Right
            texture_coordinates[1].x = x_left;       // Right --> Left
            texture_coordinates[2].x = x_left;       // Right --> Left
            texture_coordinates[3].x = x_right;      // Left --> Right
        }

        // Switch top and bottom texture coordinates to flip sprite vertically
        if (render_config.flip_vertically) {
            f32 y_top = texture_coordinates[1].y;    // Top right
            f32 y_bottom = texture_coordinates[2].y; // Bottom right
            texture_coordinates[0].y = y_bottom;     // Top --> Bottom
            texture_coordinates[1].y = y_bottom;     // Top --> Bottom
            texture_coordinates[2].y = y_top;        // Bottom --> Top
            texture_coordinates[3].y = y_top;        // Bottom --> Top
        }

        // Switch top left and bottom right texture coordinates to flip sprite diagonally
        if (render_config.flip_diagonally) {
            glm::vec2 top_left = texture_coordinates[0];
            // glm::vec2 top_right = texture_coordinates[1];
            glm::vec2 bottom_right = texture_coordinates[2];
            // glm::vec2 bottom_left = texture_coordinates[3];
            texture_coordinates[0] = bottom_right;     //
            // texture_coordinates[1] = y_bottom;     //
            texture_coordinates[2] = top_left;        //
            // texture_coordinates[3] = y_top;        //
        }

        // Render the flipped sprite
        renderer->render_quad(quad, texture_coordinates);
    }
}