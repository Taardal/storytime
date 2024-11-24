#pragma once

#include <nlohmann/json.hpp>
#include <any>

namespace Storytime {
    using TiledPropertyValue = std::variant<bool, int, float, std::string, nlohmann::json>;

    struct TiledProperty {
        std::string name;
        std::string type;
        std::string propertytype;
        TiledPropertyValue value;
    };
}
