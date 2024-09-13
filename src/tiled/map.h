#pragma once

#include "tileset.h"
#include "layer.h"

namespace Storytime {
    struct TiledMap {
        int height;
        bool infinite;
        std::vector<TiledLayer> layers;
        int next_layer_id;
        int next_object_id;
        std::string orientation;
        std::string render_order;
        std::vector<TiledTileset> tilesets;
        std::vector<TiledTilesetRef> tileset_refs;
        int tile_height;
        int tile_width;
        std::string type;
        std::string version;
        int width;

        static TiledMap create(const std::string& json);
    };
}
