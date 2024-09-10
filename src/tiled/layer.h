#pragma once

#include "object.h"

namespace Storytime {
    struct Layer {
        std::vector<Property> properties;
        std::vector<Layer> layers;
        std::vector<Object> objects;
        std::vector<int> data;
        std::string name;
        std::string type;
        std::string draw_order;
        std::string image;
        int id;
        int x;
        int y;
        int width;
        int height;
        int opacity;
        bool visible;
    };
}
