#include "st_spritesheet.h"
#include "st_sprite.h"
#include "st_animation.h"

namespace Storytime {
    Spritesheet::Spritesheet(const SpritesheetConfig& config) : config(config) {
    }

    Shared<Texture> Spritesheet::get_texture() const {
        return config.texture;
    }

    Sprite Spritesheet::get_sprite(u32 column, u32 row, u32 width, u32 height) const {

        f32 spritesheet_width = (f32) config.texture->get_width();
        f32 spritesheet_height = (f32) config.texture->get_height();

        f32 x = column * (f32) width;
        f32 y = row * (f32) height;

        f32 x_left = x / spritesheet_width;
        f32 x_right = (x + (f32) width) / spritesheet_width;

        f32 y_top = y / spritesheet_height;
        f32 y_bottom = (y + (f32) height) / spritesheet_height;

        glm::vec4 texture_rectangle{ x_left, y_top, x_right, y_bottom };

        return Sprite({
            .spritesheet_texture = config.texture,
            .spritesheet_texture_rectangle = texture_rectangle,
            .spritesheet_row = row,
            .spritesheet_column = column,
            .width = width,
            .height = height,
        });
    }

    Animation Spritesheet::get_animation(u32 row, u32 column_count, u32 sprite_width, u32 sprite_height) const {
        std::vector<AnimationFrame> frames;
        frames.reserve(column_count);
        for (int column = 0; column < column_count; ++column) {
            frames.push_back({
                .sprite = get_sprite(column, row, sprite_width, sprite_height),
            });
        }
        return Animation({
            .frames = frames,
        });
    }
}
