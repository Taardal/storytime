#pragma once

#include "st_tiled_object.h"
#include "st_tiled_tileset.h"

namespace Storytime {
    struct TiledObjectTemplate {
        TiledObject object{};
        TiledTilesetRef tileset{};
        std::string type = "";

        static TiledObjectTemplate create(const std::string& json);
    };
}
