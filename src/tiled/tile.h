#pragma once

#include "object.h"

namespace Storytime {
    struct TiledAnimationFrame {
        int duration;
        int tileid;
    };

    struct TiledTile {
        std::vector<TiledAnimationFrame> animation;
        int id;
        TiledObjectGroup objectgroup;

        static TiledTile create(const std::string& json);
    };
}
