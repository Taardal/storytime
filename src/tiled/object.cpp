#include "object.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledProperty& property);

    void from_json(const nlohmann::json& json, TiledObject& object) {
        object.name = json.at("name").get<std::string>();
        object.type = json.at("type").get<std::string>();
        object.rotation = json.at("rotation").get<float>();
        object.id = json.at("id").get<int>();
        object.x = json.at("x").get<int>();
        object.y = json.at("y").get<int>();
        object.width = json.at("width").get<int>();
        object.height = json.at("height").get<int>();
        object.visible = json.at("visible").get<bool>();
        if (json.contains("properties")) {
            object.properties = json.at("properties").get<std::vector<TiledProperty>>();
        }
    }
}
