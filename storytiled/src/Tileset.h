#pragma once

#include <string>

namespace storytiled
{
    struct Tileset
    {
        std::string Name;
        std::string Image;
        int FirstGlobalId;
        int ImageWidth;
        int ImageHeight;
        int TileWidth;
        int TileHeight;
        int TileCount;
        int Columns;
        int Margin;
        int Spacing;

        Tileset();
    };

}
