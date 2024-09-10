#include "layer.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, Property& property);

    extern void from_json(const nlohmann::json&, Object& object);

    void from_json(const nlohmann::json& json, Layer& layer) {
        layer.name = json.at("name").get<std::string>();
        layer.type = json.at("type").get<std::string>();
        layer.id = json.at("id").get<int>();
        layer.x = json.at("x").get<int>();
        layer.y = json.at("y").get<int>();
        layer.opacity = json.at("opacity").get<int>();
        layer.visible = json.at("visible").get<bool>();
        if (layer.type == "objectgroup") {
            layer.objects = json.at("objects").get<std::vector<Object>>();
            layer.draw_order = json.at("draworder").get<std::string>();
        } else if (layer.type == "tilelayer") {
            layer.data = json.at("data").get<std::vector<int>>();
            layer.width = json.at("width").get<int>();
            layer.height = json.at("height").get<int>();
        } else if (layer.type == "image") {
            layer.image = json.at("image").get<std::string>();
        } else if (layer.type == "group") {
            layer.layers = json.at("layers").get<std::vector<Layer>>();
        }
        if (json.contains("properties")) {
            layer.properties = json.at("properties").get<std::vector<Property>>();
        }
    }
}
