#pragma once

#include "st_tiled_tile.h"

namespace Storytime {
    struct TiledTileset {
        int columns = 0;
        int firstgid = 0;
        std::string image = "";
        int imageheight = 0;
        int imagewidth = 0;
        int margin = 0;
        std::string name = "";
        int spacing = 0;
        std::vector<TiledTile> tiles{};
        int tilecount = 0;
        int tileheight = 0;
        int tilewidth = 0;

        static TiledTileset create(const std::string& json);
    };

    struct TiledTilesetRef {
        int firstgid = 0;
        std::string source = "";

        static TiledTilesetRef create(const std::string& json);
    };
}
