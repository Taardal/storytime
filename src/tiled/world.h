#pragma once

#include "tileset.h"
#include "layer.h"

namespace Storytime {
    struct World {
        std::vector<Tileset> tilesets;
        std::vector<Layer> layers;
        std::string type;
        std::string orientation;
        std::string render_order;
        std::string version;
        int width;
        int height;
        int tile_width;
        int tile_height;
        int next_layer_id;
        int next_object_id;
        bool infinite;

        static World create(const std::string& json);
    };
}
