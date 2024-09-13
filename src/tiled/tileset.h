#pragma once

namespace Storytime {
    struct TiledTileset {
        int columns;
        int firstgid;
        std::string image;
        int image_height;
        int image_width;
        int margin;
        std::string name;
        int spacing;
        int tile_count;
        int tile_height;
        int tile_width;

        static TiledTileset create(const std::string& json);
    };

    struct TiledTilesetRef {
        int firstgid;
        std::string source;

        static TiledTilesetRef create(const std::string& json);
    };
}
