#pragma once

#include "graphics/st_renderer.h"
#include "graphics/st_texture.h"

namespace Storytime {
    struct BitmapFontConfig {
        Renderer* renderer = nullptr;
        Shared<Texture> texture = nullptr;
        f32 glyph_width = 8.0f;
        f32 glyph_height = 16.0f;
        std::map<const char, std::pair<u32, u32>> coordinates;
    };

    struct BitmapTextConfig {
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        f32 font_size = 0.0f;
        bool debug = false;
    };

    class BitmapFont {
    private:
        BitmapFontConfig config;

    public:
        BitmapFont(const BitmapFontConfig& config);

        void render_text(const std::string& text, const BitmapTextConfig& text_config = {}) const;
    };
}
