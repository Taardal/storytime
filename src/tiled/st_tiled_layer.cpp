#include "st_tiled_layer.h"
#include "st_tiled_template.h"

#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json&, TiledProperty&);

    extern void from_json(const nlohmann::json&, TiledObject&);

    extern void from_json(const nlohmann::json&, TiledObjectTemplateRef&);

    void from_json(const nlohmann::json& json, TiledLayer& data) {
        data.name = json.at("name").get<std::string>();
        data.type = json.at("type").get<std::string>();
        data.id = json.at("id").get<int>();
        data.x = json.at("x").get<int>();
        data.y = json.at("y").get<int>();
        data.opacity = json.at("opacity").get<int>();
        data.visible = json.at("visible").get<bool>();
        if (data.type == "objectgroup") {
            if (json.contains("objects")) {
                for (const nlohmann::json& object_json : json.at("objects")) {
                    bool is_object_template_ref = object_json.contains("template");
                    if (is_object_template_ref) {
                        data.object_template_refs.push_back(object_json.get<TiledObjectTemplateRef>());
                    } else {
                        data.objects.push_back(object_json.get<TiledObject>());
                    }
                }
            }
            data.draworder = json.at("draworder").get<std::string>();
        } else if (data.type == "tilelayer") {
            data.data = json.at("data").get<std::vector<int>>();
            data.width = json.at("width").get<int>();
            data.height = json.at("height").get<int>();
        } else if (data.type == "image") {
            data.image = json.at("image").get<std::string>();
        } else if (data.type == "group") {
            data.layers = json.at("layers").get<std::vector<TiledLayer>>();
        }
        if (json.contains("properties")) {
            data.properties = json.at("properties").get<std::vector<TiledProperty>>();
        }
    }

    TiledLayer TiledLayer::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledLayer>();
    }

    const TiledProperty& TiledLayer::get_property(const std::string& name) const {
        return get_tiled_property(properties, name);
    }

    const TiledProperty* TiledLayer::try_get_property(const std::string& name) const {
        return try_get_tiled_property(properties, name);
    }
}
