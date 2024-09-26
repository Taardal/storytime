#pragma once

#include "property.h"

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
}
