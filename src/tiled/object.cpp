#include "object.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledProperty& property);

    void from_json(const nlohmann::json& json, TiledObject& object) {
        object.height = json.at("height").get<int>();
        object.id = json.at("id").get<int>();
        object.name = json.at("name").get<std::string>();
        object.point = json.at("point").get<bool>();
        if (json.contains("properties")) {
            object.properties = json.at("properties").get<std::vector<TiledProperty>>();
        }
        object.rotation = json.at("rotation").get<float>();
        object.type = json.at("type").get<std::string>();
        object.visible = json.at("visible").get<bool>();
        object.width = json.at("width").get<int>();
        object.x = json.at("x").get<int>();
        object.y = json.at("y").get<int>();
    }
}
