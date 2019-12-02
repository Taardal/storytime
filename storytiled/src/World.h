#pragma once

#include <string>

namespace storytiled {

    struct World {
        std::string tiledVersion;
        std::string type;
        int width;
        int height;
        int tileWidth;
        int tileHeight;
        bool infinite;

        static World fromJson(const std::string& json);
    };

}


