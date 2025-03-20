#pragma once

namespace Storytime {
    struct TiledEnum {
        int id;
        std::string name;
        std::string storage_type;
        std::string type;
        std::vector<std::string> values;
        bool values_as_flags;
    };
}