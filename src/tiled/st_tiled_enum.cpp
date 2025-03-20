#include "st_tiled_enum.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledEnum& data) {
        data.id = json.at("id").get<int>();
        data.name = json.at("name").get<std::string>();
        data.storage_type = json.at("storageType").get<std::string>();
        data.type = json.at("type").get<std::string>();
        data.values = json.at("values").get<std::vector<std::string>>();
        data.values_as_flags = json.at("valuesAsFlags").get<bool>();
    }
}