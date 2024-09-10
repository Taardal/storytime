#pragma once

namespace Storytime {
    struct Tileset {
        std::string name;
        std::string image;
        int first_global_id;
        int image_width;
        int image_height;
        int tile_width;
        int tile_height;
        int tile_count;
        int columns;
        int margin;
        int spacing;
    };
}
