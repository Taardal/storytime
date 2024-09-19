#pragma once

#include "object.h"

namespace Storytime {
    struct TiledLayer {
        std::vector<TiledProperty> properties;
        std::vector<TiledLayer> layers;
        std::vector<TiledObject> objects;
        std::vector<int> data;
        std::string name;
        std::string type;
        std::string draworder;
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
