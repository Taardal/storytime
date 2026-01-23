#pragma once

namespace Storytime {
    struct TiledEnum {
        int id = 0;
        std::string name = "";
        std::string storage_type = "";
        std::string type = "";
        std::vector<std::string> values{};
        bool values_as_flags = false;
    };
}
