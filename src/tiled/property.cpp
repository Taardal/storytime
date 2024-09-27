#include "property.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    void from_json(const nlohmann::json& json, TiledProperty& data) {
        data.name = json.at("name").get<std::string>();
        data.type = json.at("type").get<std::string>();

        auto value_json = json.at("value");
        if (data.type == "bool") {
            data.value = value_json.get<bool>();
        }
        if (data.type == "float") {
            data.value = value_json.get<float>();
        }
        if (data.type == "int" || data.type == "object") {
            data.value = value_json.get<int>();
        }
        if (data.type == "string" || data.type == "color" || data.type == "file") {
            data.value = value_json.get<std::string>();
        }
    }
}