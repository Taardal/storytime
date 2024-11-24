#pragma once

#include "tiled_object.h"
#include "tiled_property.h"

namespace Storytime {
    struct TiledAnimationFrame {
        int duration;
        int tileid;
    };

    struct TiledTile {
        std::vector<TiledAnimationFrame> animation;
        int id;
        TiledObjectGroup objectgroup;
        std::vector<TiledProperty> properties;

        static TiledTile create(const std::string& json);
    };
}
