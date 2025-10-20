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
        glm::vec3 position = { render_config.position.x, render_config.position.y, render_config.position.z };
        glm::vec3 size = { (f32) config.width, (f32) config.height, 0.0f };

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(render_config.rotation_deg), { 0.0f, 0.0f, 1.0f });
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), size);

        glm::mat4 model = translation * rotation * scale;

        Quad quad = {
            .model = model,
            .color = render_config.color,
            .texture = config.spritesheet_texture,
            .texture_rectangle = config.spritesheet_texture_rectangle,
        };

        bool flipped = render_config.flipped_horizontally || render_config.flipped_vertically || render_config.flipped_diagonally;
        if (!flipped) {
            renderer->render_quad(quad);
            return;
        }

        // Sprite should be flipped
        // Copy the texture coordinates to preserve the normal/unflipped ones
        glm::vec4 texture_coordinates = config.spritesheet_texture_rectangle;

        // Switch left and right texture coordinates to flip sprite horizontally
        if (render_config.flipped_horizontally || render_config.flipped_diagonally) {
            f32 x_left = texture_coordinates.x;
            f32 x_right = texture_coordinates.z;
            texture_coordinates.x = x_right;
            texture_coordinates.z = x_left;
        }

        // Switch top and bottom texture coordinates to flip sprite vertically
        if (render_config.flipped_vertically || render_config.flipped_diagonally) {
            f32 y_top = texture_coordinates.y;
            f32 y_bottom = texture_coordinates.w;
            texture_coordinates.x = y_bottom;
            texture_coordinates.z = y_top;
        }

        // Render the flipped sprite
        quad.texture_rectangle = texture_coordinates;
        renderer->render_quad(quad);
    }
}