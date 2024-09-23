#pragma once

#include "object.h"

namespace Storytime {
    struct TiledAnimationFrame {
        int duration;
        int tileid;
    };

    struct TiledObjectGroup {
        std::string draworder;
        int id;
        std::string name;
        std::vector<TiledObject> objects;
        float opacity;
        std::string type;
        bool visible;
        int x;
        int y;
    };

    struct TiledTile {
        std::vector<TiledAnimationFrame> animation;
        int id;
        TiledObjectGroup objectgroup;

        static TiledTile create(const std::string& json);
    };
}
