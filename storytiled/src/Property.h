#pragma once

#include <string>

namespace storytiled
{
    struct Property
    {
        std::string Name;
        std::string Type;
        std::string Value;

        Property();
    };
}