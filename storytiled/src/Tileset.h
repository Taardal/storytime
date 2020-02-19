#pragma once

#include <string>

namespace storytiled {

    struct Tileset {
        std::string name;
        std::string imagePath;
        int firstGlobalId;
        int numberOfColumns;
        int numberOfTiles;
        int tileWidth;
        int imageWidth;
        int imageHeight;
        int tileHeight;
        int margin;
        int spacing;
    };

}
