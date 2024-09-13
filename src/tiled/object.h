#pragma once

#include "property.h"

namespace Storytime {
    struct TiledObject {
        std::vector<TiledProperty> properties;
        std::string name;
        std::string type;
        float rotation;
        int id;
        int x;
        int y;
        int width;
        int height;
        bool visible;
    };
}
