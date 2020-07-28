#pragma once

#include "Property.h"
#include <string>
#include <vector>

namespace storytiled
{
    struct Object
    {
        std::vector<Property> Properties;
        std::string Name;
        std::string Type;
        float Rotation;
        int Id;
        int X;
        int Y;
        int Width;
        int Height;
        bool Visible;

        Object();
    };
}