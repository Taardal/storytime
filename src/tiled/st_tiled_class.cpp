#include "st_tiled_class.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledClassMember& data) {
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
        }
    }

    extern void from_json(const nlohmann::json& json, TiledClass& data) {
        if (json.contains("color")) {
            data.color = json.at("color").get<std::string>();
        }
        if (json.contains("draw_fill")) {
            data.draw_fill = json.at("drawFill").get<bool>();
        }
        if (json.contains("id")) {
            data.id = json.at("id").get<int>();
        }
        if (json.contains("members")) {
            data.members = json.at("members").get<std::vector<TiledClassMember>>();
        }
        if (json.contains("name")) {
            data.name = json.at("name").get<std::string>();
        }
        if (json.contains("type")) {
            data.type = json.at("type").get<std::string>();
        }
        if (json.contains("use_as")) {
            data.use_as = json.at("useAs").get<std::vector<std::string>>();
        }
    }
}