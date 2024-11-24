#pragma once

#include "tiled_class.h"
#include "tiled_enum.h"

namespace Storytime {
    using TiledProjectPropertyType = std::variant<TiledClass, TiledEnum>;

    struct TiledProject {
        std::vector<TiledProjectPropertyType> property_types;

        static TiledProject create(const std::string& json);
    };
}