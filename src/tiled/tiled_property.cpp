#include "tiled_property.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    void from_json(const nlohmann::json& json, TiledProperty& data) {
        data.name = json.at("name").get<std::string>();
        data.type = json.at("type").get<std::string>();
        auto value_json = json.at("value");
        if (data.type == "bool") {
            data.value = value_json.get<bool>();
        } else if (data.type == "float") {
            data.value = value_json.get<float>();
        } else if (data.type == "int" || data.type == "object") {
            data.value = value_json.get<int>();
        } else if (data.type == "string" || data.type == "color" || data.type == "file") {
            data.value = value_json.get<std::string>();
        } else if (data.type == "class" || data.type == "enum") {
            data.value = value_json;
        }
        if (json.contains("propertytype")) {
            data.propertytype = json.at("propertytype").get<std::string>();
        }
    }
}
