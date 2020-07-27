#pragma once

#include "Tileset.h"
#include "Layer.h"
#include <string>
#include <vector>

namespace storytiled
{
    struct World
    {
        std::vector<Tileset> Tilesets;
        std::vector<Layer> Layers;
        std::string Type;
        std::string Orientation;
        std::string RenderOrder;
        float Version;
        int Width;
        int Height;
        int TileWidth;
        int TileHeight;
        int NextLayerId;
        int NextObjectId;
        bool Infinite;

        World();

        static World FromJson(const std::string& json);
    };
}


