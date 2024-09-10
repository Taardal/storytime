#include "property.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    void from_json(const nlohmann::json& json, Property& property) {
        property.name = json.at("name").get<std::string>();
        property.type = json.at("type").get<std::string>();
        property.value = json.at("value").get<std::string>();
    }
}