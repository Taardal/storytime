#include "bitmap_font.h"

namespace Storytime {
    BitmapFont::BitmapFont(BitmapFontConfig config) : config(std::move(config)) {}

    void BitmapFont::render_text(const std::string& text, const BitmapTextConfig& text_config) {
        float factor = 5.0f;

        float gw = 6.0f * factor;
        float gh = 12.0f * factor;
        float ts = config.texture->get_width() * factor;

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

            std::array<glm::vec2, 4> c;
            c[0] = {xat, yat};
            c[1] = {xbt, yat};
            c[2] = {xbt, ybt};
            c[3] = {xat, ybt};

            Quad quad;
            quad.texture = config.texture;
            quad.size = {size_x, size_y};
            quad.position = position;
            config.renderer->render_quad(quad, c);

            position.x += size_x;
        }
    }
}
