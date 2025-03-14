#include "tiled_project.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json&, TiledClass&);

    extern void from_json(const nlohmann::json&, TiledEnum&);

    extern void from_json(const nlohmann::json& json, TiledProject& data) {
        if (json.contains("propertyTypes")) {
            for (auto property_type_json : json.at("propertyTypes")) {
                if (!property_type_json.contains("type")) {
                    continue;
                }
                if (property_type_json.at("type") == "class") {
                    data.property_types.emplace_back(property_type_json.get<TiledClass>());
                } else if (property_type_json.at("type") == "enum") {
                    data.property_types.emplace_back(property_type_json.get<TiledEnum>());
                }
            }
        }
    }

    TiledProject Storytime::TiledProject::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledProject>();
    }
}
