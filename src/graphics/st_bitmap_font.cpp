#include "st_bitmap_font.h"

namespace Storytime {
    BitmapFont::BitmapFont(const BitmapFontConfig& config) : config(config) {}

    void BitmapFont::render_text(const std::string& text, const BitmapTextConfig& text_config) const {
        float gw = config.glyph_width;
        float gh = config.glyph_height;
        float ts = config.texture->get_width();

        f32 size_x = text_config.font_size * (gw / gh);
        f32 size_y = text_config.font_size;

        glm::vec3 position = text_config.position;

        for (int i = 0; i < text.size(); ++i) {
            const char value = text[i];

            if (value == '\n') {
                position.y += size_y;
                position.x = text_config.position.x;
                continue;
            }

            auto it = config.coordinates.find(value);

            u32 col = it->second.first;
            u32 row = it->second.second;

            float xa = static_cast<float>(col) * gw;
            float xb = xa + gw;

            float ya = static_cast<float>(row) * gh;
            float yb = ya + gh;

            float xat = xa / ts;
            float xbt = xb / ts;

            float yat = ya / ts;
            float ybt = yb / ts;

            glm::vec4 c = { xat, yat, xbt, ybt };

            glm::vec3 p = { position.x, position.y, 0.0f };
            glm::vec3 s = { size_x, size_y, 0.0f };

            glm::mat4 translation = glm::translate(glm::mat4(1.0f), p);
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0.0f, 0.0f, 1.0f });
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), s);

            glm::mat4 model = translation * rotation * scale;

            Quad quad{};
            quad.texture = config.texture;
            quad.model = model;

            if (text_config.debug) {
                Quad debug_quad = quad;
                debug_quad.color = { (f32) i, (f32) i, (f32) i, 0.5f };
                config.renderer->render_quad(debug_quad);
            }

            config.renderer->render_quad(quad, c);

            position.x += size_x;
        }
    }
}
