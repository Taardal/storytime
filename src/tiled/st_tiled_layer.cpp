#include "st_tiled_layer.h"
#include "st_tiled_template.h"

#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json&, TiledProperty&);

    extern void from_json(const nlohmann::json&, TiledObject&);

    void from_json(const nlohmann::json& json, TiledLayer& data) {
        if (json.contains("data")) {
            data.data = json.at("data").get<std::vector<int>>();
        }
        if (json.contains("draworder")) {
            data.draworder = json.at("draworder").get<std::string>();
        }
        if (json.contains("height")) {
            data.height = json.at("height").get<int>();
        }
        if (json.contains("id")) {
            data.id = json.at("id").get<int>();
        }
        if (json.contains("image")) {
            data.image = json.at("image").get<std::string>();
        }
        if (json.contains("layers")) {
            data.layers = json.at("layers").get<std::vector<TiledLayer>>();
        }
        if (json.contains("locked")) {
            data.locked = json.at("locked").get<bool>();
        }
        if (json.contains("name")) {
            data.name = json.at("name").get<std::string>();
        }
        if (json.contains("offsetx")) {
            data.offsetx = json.at("offsetx").get<float>();
        }
        if (json.contains("offsety")) {
            data.offsety = json.at("offsety").get<float>();
        }
        if (json.contains("objects")) {
            const nlohmann::json& objects_json = json.at("objects");
            data.objects.reserve(objects_json.size());
            for (const nlohmann::json& object_json : objects_json) {
                data.objects.push_back(object_json.get<TiledObject>());
            }
        }
        if (json.contains("opacity")) {
            data.opacity = json.at("opacity").get<int>();
        }
        if (json.contains("parallaxx")) {
            data.parallaxx = json.at("parallaxx").get<float>();
        }
        if (json.contains("parallaxy")) {
            data.parallaxy = json.at("parallaxy").get<float>();
        }
        if (json.contains("properties")) {
            data.properties = json.at("properties").get<std::vector<TiledProperty>>();
        }
        if (json.contains("type")) {
            data.type = json.at("type").get<std::string>();
        }
        if (json.contains("visible")) {
            data.visible = json.at("visible").get<bool>();
        }
        if (json.contains("width")) {
            data.width = json.at("width").get<int>();
        }
        if (json.contains("x")) {
            data.x = json.at("x").get<float>();
        }
        if (json.contains("y")) {
            data.y = json.at("y").get<float>();
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

    bool TiledLayer::is_parallax() const {
        return parallaxx != default_parallax || parallaxy != default_parallax;
    }
}
