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

        //
        // Problem:
        // The renderer will offset each quad left and up by half its size to put its origin in the center,
        // but this will make each quad appear slightly off-position compared to Tiled Editor.
        //
        // Solution:
        // Offset the position of each sprite right and down by half its size so that the quad offsets done by
        // the renderer will make them appear at the correct position
        //
        position.x += size_x / 2;
        position.y += size_y / 2;

        for (int i = 0; i < text.size(); ++i) {
            const char value = text[i];

            if (value == '\n') {
                position.y += size_y;
                position.x = text_config.position.x + size_x / 2;
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

            std::array<TextureCoordinate, 4> c;
            c[0] = {xat, yat};
            c[1] = {xbt, yat};
            c[2] = {xbt, ybt};
            c[3] = {xat, ybt};

            if (text_config.debug) {
                Quad debug_quad;
                debug_quad.color = { (f32) i, (f32) i, (f32) i, 0.5f };
                debug_quad.size = {size_x, size_y};
                debug_quad.position = position;
                config.renderer->render_quad(debug_quad);
            }

            Quad quad;
            quad.texture = config.texture;
            quad.size = {size_x, size_y};
            quad.position = position;
            config.renderer->render_quad(quad, c);

            position.x += size_x;
        }
    }
}
