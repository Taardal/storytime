#pragma once

#include "Object.h"
#include <vector>

namespace storytiled
{
    struct Layer
    {
        std::vector<Property> Properties;
        std::vector<Layer> Layers;
        std::vector<Object> Objects;
        std::vector<int> Data;
        std::string Name;
        std::string Type;
        std::string DrawOrder;
        std::string Image;
        int Id;
        int X;
        int Y;
        int Width;
        int Height;
        int Opacity;
        bool Visible;

        Layer();
    };
}