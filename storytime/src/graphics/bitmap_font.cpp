#include "bitmap_font.h"

namespace Storytime {
    BitmapFont::BitmapFont(BitmapFontConfig config) : config(std::move(config)) {}

    void BitmapFont::render_text(const std::string& text, const BitmapTextConfig& text_config) {
        float factor = 5.0f;

        float gw = 6.0f * factor;
        float gh = 12.0f * factor;
        float ts = config.texture->get_width() * factor;

        glm::vec3 position = text_config.position;

        for (int i = 0; i < text.size(); ++i) {
            const char value = text[i];

            if (value == '\n') {
                position.y += gh;
                position.x = text_config.position.x;
                continue;
            }

            auto it = config.coordinates.find(value);
            ST_ASSERT_THROW(it != config.coordinates.end());

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

            std::vector<glm::vec2> c = {
                {xat, yat},
                {xbt, yat},
                {xbt, ybt},
                {xat, ybt},
            };

            Quad quad;
            quad.texture = config.texture;
            quad.size = {gw, gh};
            quad.color = {1.0f, 0.0f, 0.0f, 1.0f};
            quad.position = position;
            config.renderer->submit_quad(quad, c);

            position.x += gw;
        }
    }
}
