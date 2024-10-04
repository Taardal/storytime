#pragma once

#include "tileset.h"
#include "layer.h"

namespace Storytime {
    struct TiledMap {
        int height;
        bool infinite;
        std::vector<TiledLayer> layers;
        int nextlayerid;
        int nextobjectid;
        std::string orientation;
        std::string renderorder;
        std::vector<TiledTileset> tilesets;
        std::vector<TiledTilesetRef> tilesetrefs;
        int tileheight;
        int tilewidth;
        std::string type;
        std::string version;
        int width;

        static TiledMap create(const std::string& json);
    };
}
