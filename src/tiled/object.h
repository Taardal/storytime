#pragma once

#include "property.h"

namespace Storytime {
    struct Object {
        std::vector<Property> properties;
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
