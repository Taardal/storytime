#include "spritesheet.h"

namespace Storytime {
    Sprite::Sprite(const SpriteConfig& config) : config(config) {
        u32 start_row = config.row > 0 ? config.row : config.start_row;
        u32 start_column = config.column > 0 ? config.column : config.start_column;

        u32 end_row = config.row > 0 ? config.row : config.end_row;
        u32 end_column = config.column > 0 ? config.column : config.end_column;

        f32 sprite_width = (f32) config.width;
        f32 sprite_height = (f32) config.height;

        f32 spritesheet_width = (f32) config.spritesheet_texture->get_width();
        f32 spritesheet_height = (f32) config.spritesheet_texture->get_height();

        for (u32 row = start_row; row <= end_row; ++row) {
            for (u32 column = start_column; column <= end_column; ++column) {
                f32 x = column * sprite_width;
                f32 y = row * sprite_height;

                f32 x_left = x / spritesheet_width;
                f32 x_right = (x + sprite_width) / spritesheet_width;

                f32 y_top = y / spritesheet_height;
                f32 y_bottom = (y + sprite_height) / spritesheet_height;

                std::array<TextureCoordinate, 4> texture_coordinates;
                texture_coordinates[0] = {x_left, y_top};     // Top left
                texture_coordinates[1] = {x_right, y_top};    // Top right
                texture_coordinates[2] = {x_right, y_bottom}; // Bottom right
                texture_coordinates[3] = {x_left, y_bottom};  // Bottom left

                texture_coordinate_list.push_back(texture_coordinates);
            }
        }
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
            debug_quad.color = { 1.0f, 0.0f, 0.0f, 1.0f };
            debug_quad.size = { quad_width, quad_height };
            debug_quad.position = { quad_x, quad_y, 0.0f };
            renderer.render_quad(debug_quad);
        }

        Quad quad;
        quad.texture = config.spritesheet_texture;
        quad.size = { quad_width, quad_height };
        quad.position = { quad_x, quad_y, 0.0f };
        renderer.render_quad(quad, texture_coordinate_list[0]);
    }
}

namespace Storytime {
    Spritesheet::Spritesheet(SpritesheetConfig config) : config(std::move(config)) {
    }

    Sprite Spritesheet::get_sprite(SpriteConfig& sprite_config) const {
        sprite_config.spritesheet_texture = config.texture;
        return Sprite(sprite_config);
    }
}
