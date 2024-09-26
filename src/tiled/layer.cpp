#include "layer.h"
#include <nlohmann/json.hpp>

#include "template.h"

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledProperty& property);

    extern void from_json(const nlohmann::json&, TiledObject& object);

    extern void from_json(const nlohmann::json&, TiledObjectTemplateRef& object_template_ref);

    void from_json(const nlohmann::json& json, TiledLayer& layer) {
        layer.name = json.at("name").get<std::string>();
        layer.type = json.at("type").get<std::string>();
        layer.id = json.at("id").get<int>();
        layer.x = json.at("x").get<int>();
        layer.y = json.at("y").get<int>();
        layer.opacity = json.at("opacity").get<int>();
        layer.visible = json.at("visible").get<bool>();
        if (layer.type == "objectgroup") {


            // layer.objects = json.at("objects").get<std::vector<TiledObject>>();
            if (json.contains("objects")) {
                for (const nlohmann::json& object_json : json.at("objects")) {
                    bool is_object_template_ref = object_json.contains("template");
                    if (is_object_template_ref) {
                        layer.object_template_refs.push_back(object_json.get<TiledObjectTemplateRef>());
                    } else {
                        layer.objects.push_back(object_json.get<TiledObject>());
                    }
                }
            }


            layer.draworder = json.at("draworder").get<std::string>();
        } else if (layer.type == "tilelayer") {
            layer.data = json.at("data").get<std::vector<int>>();
            layer.width = json.at("width").get<int>();
            layer.height = json.at("height").get<int>();
        } else if (layer.type == "image") {
            layer.image = json.at("image").get<std::string>();
        } else if (layer.type == "group") {
            layer.layers = json.at("layers").get<std::vector<TiledLayer>>();
        }
        if (json.contains("properties")) {
            layer.properties = json.at("properties").get<std::vector<TiledProperty>>();
        }
    }
}
