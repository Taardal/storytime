#pragma once

#include "tiled_property.h"

namespace Storytime {
    struct TiledObject {
        int gid;
        int height;
        int id;
        std::string name;
        bool point;
        std::vector<TiledProperty> properties;
        float rotation;
        std::string type;
        bool visible;
        int width;
        float x;
        float y;
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
}
