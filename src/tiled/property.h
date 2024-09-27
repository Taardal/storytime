#pragma once

#include <any>

namespace Storytime {

    using TiledPropertyValue = std::variant<bool, int, float, std::string>;

    struct TiledProperty {
        std::string name;
        std::string type;
        TiledPropertyValue value;
    };
}
