#pragma once

#include "st_tiled_tile.h"

namespace Storytime {
    struct TiledTileset {
        int columns;
        int firstgid;
        std::string image;
        int imageheight;
        int imagewidth;
        int margin;
        std::string name;
        int spacing;
        std::vector<TiledTile> tiles;
        int tilecount;
        int tileheight;
        int tilewidth;

        static TiledTileset create(const std::string& json);
    };

    struct TiledTilesetRef {
        int firstgid;
        std::string source;

        static TiledTilesetRef create(const std::string& json);
    };
}
