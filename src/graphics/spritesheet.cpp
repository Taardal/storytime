#include "spritesheet.h"

namespace Storytime {
    Sprite::Sprite(SpriteConfig config) : config(std::move(config)) {
    }
}

namespace Storytime {
    Spritesheet::Spritesheet(SpritesheetConfig config) : config(std::move(config)) {
    }

    const SpritesheetConfig& Spritesheet::get_config() const {
        return config;
    }

    Sprite Spritesheet::get_sprite() const {
        SpriteConfig sprite_config = {
            .texture = config.texture,
            .width = config.sprite_width,
            .height = config.sprite_height,
        };
        return Sprite(sprite_config);
    }

    void Spritesheet::set_sprite_width(u32 sprite_width) {
        config.sprite_width = sprite_width;
    }

    void Spritesheet::set_sprite_height(u32 sprite_height) {
        config.sprite_height = sprite_height;
    }

    void Spritesheet::set_sprite_size(u32 sprite_size) {
        config.sprite_width = sprite_size;
        config.sprite_height = sprite_size;
    }

    void Spritesheet::render(Renderer* renderer, const glm::vec3& position, const glm::vec2& frame, const glm::vec2& size) const {
        float factor = 1.0f;

        float gw = config.sprite_width * factor;
        float gh = config.sprite_height * factor;
        float tw = config.texture->get_width() * factor;
        float th = config.texture->get_height() * factor;

        u32 col = frame.x;
        u32 row = 0;

        float xa = static_cast<float>(col) * gw;
        float xb = xa + gw;

        float ya = static_cast<float>(row) * gh;
        float yb = ya + gh;

        float xat = xa / tw;
        float xbt = xb / tw;

        float yat = ya / th;
        float ybt = yb / th;

        std::vector<glm::vec2> c = {
            {xat, yat},
            {xbt, yat},
            {xbt, ybt},
            {xat, ybt},
        };

        Quad quad;
        quad.texture = config.texture;
        quad.size = size;
        // quad.color = {1.0f, 0.0f, 0.0f, 1.0f};
        quad.position = position;
        renderer->submit_quad(quad, c);
    }
}
