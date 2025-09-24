#include "st_sprite.h"

namespace Storytime {
    Sprite::Sprite(const SpriteConfig& config) : config(config) {
        ST_ASSERT(config.spritesheet_texture != nullptr, "Sprite must have a texture");
    }

    bool Sprite::is_valid() const {
        return config.spritesheet_texture != nullptr && config.width > 0 && config.height > 0;
    }

    void Sprite::invalidate() {
        config.spritesheet_texture = nullptr;
        config.width = 0;
        config.height = 0;
    }

    Shared<Texture> Sprite::get_texture() {
        return config.spritesheet_texture;
    }

    u32 Sprite::get_width() const {
        return config.width;
    }

    u32 Sprite::get_height() const {
        return config.height;
    }

    Size<u32> Sprite::get_size() const {
        return { config.width, config.height };
    }

    void Sprite::render(Renderer* renderer, const SpriteRenderConfig& render_config) const {
        Quad quad = {
            .texture = config.spritesheet_texture,
            .position = { render_config.position.x, render_config.position.y, 0.0f },
            .size = { (f32) config.width, (f32) config.height },
        };

        bool flipped = render_config.flipped_horizontally || render_config.flipped_vertically || render_config.flipped_diagonally;
        if (!flipped) {
            renderer->render_quad(quad, config.spritesheet_texture_coordinates);
            return;
        }

        // Sprite should be flipped
        // Copy the texture coordinates to preserve the normal/unflipped ones
        std::array<TextureCoordinate, 4> texture_coordinates = config.spritesheet_texture_coordinates;

        // Switch left and right texture coordinates to flip sprite horizontally
        if (render_config.flipped_horizontally) {
            f32 x_left = texture_coordinates[0].x;   // Top left
            f32 x_right = texture_coordinates[1].x;  // Top right
            texture_coordinates[0].x = x_right;      // Left --> Right
            texture_coordinates[1].x = x_left;       // Right --> Left
            texture_coordinates[2].x = x_left;       // Right --> Left
            texture_coordinates[3].x = x_right;      // Left --> Right
        }

        // Switch top and bottom texture coordinates to flip sprite vertically
        if (render_config.flipped_vertically) {
            f32 y_top = texture_coordinates[1].y;    // Top right
            f32 y_bottom = texture_coordinates[2].y; // Bottom right
            texture_coordinates[0].y = y_bottom;     // Top --> Bottom
            texture_coordinates[1].y = y_bottom;     // Top --> Bottom
            texture_coordinates[2].y = y_top;        // Bottom --> Top
            texture_coordinates[3].y = y_top;        // Bottom --> Top
        }

        // Switch top left and bottom right texture coordinates to flip sprite diagonally
        if (render_config.flipped_diagonally) {
            glm::vec2 top_left = texture_coordinates[0];
            glm::vec2 bottom_right = texture_coordinates[2];
            texture_coordinates[0] = bottom_right;
            texture_coordinates[2] = top_left;
        }

        // Render the flipped sprite
        renderer->render_quad(quad, texture_coordinates);
    }
}