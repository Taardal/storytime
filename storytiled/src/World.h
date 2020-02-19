#pragma once

#include "Tileset.h"
#include <string>
#include <vector>

namespace storytiled {

    struct World {
        std::vector<Tileset> tilesets;
        std::string type;
        int version;
        int id;
        int nextLayerId;
        int nextObjectId;
        int width;
        int height;
        int tileWidth;
        int tileHeight;
        bool infinite;

        static World fromJson(const std::string& json);
    };

}


