#include "st_tiled_enum.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledEnum& data) {
        if (json.contains("id")) {
            data.id = json.at("id").get<int>();
        }
        if (json.contains("name")) {
            data.name = json.at("name").get<std::string>();
        }
        if (json.contains("storageType")) {
            data.storage_type = json.at("storageType").get<std::string>();
        }
        if (json.contains("type")) {
            data.type = json.at("type").get<std::string>();
        }
        if (json.contains("values")) {
            data.values = json.at("values").get<std::vector<std::string>>();
        }
        if (json.contains("valuesAsFlags")) {
            data.values_as_flags = json.at("valuesAsFlags").get<bool>();
        }
    }
}
