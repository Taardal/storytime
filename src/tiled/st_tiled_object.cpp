#include "st_tiled_object.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledProperty& data);

    void from_json(const nlohmann::json& json, TiledObject& data) {
        if (json.contains("gid")) {
            data.gid = json.at("gid").get<int>();
        }
        if (json.contains("height")) {
            data.height = json.at("height").get<int>();
        }
        if (json.contains("id")) {
            data.id = json.at("id").get<int>();
        }
        if (json.contains("name")) {
            data.name = json.at("name").get<std::string>();
        }
        if (json.contains("point")) {
            data.point = json.at("point").get<bool>();
        }
        if (json.contains("properties")) {
            data.properties = json.at("properties").get<std::vector<TiledProperty>>();
        }
        if (json.contains("rotation")) {
            data.rotation = json.at("rotation").get<float>();
        }
        if (json.contains("template")) {
            data.template_path = json.at("template").get<std::string>();
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
            data.x = json.at("x").get<int>();
        }
        if (json.contains("y")) {
            data.y = json.at("y").get<int>();
        }
    }

    void from_json(const nlohmann::json& json, TiledObjectGroup& data) {
        if (json.contains("draworder")) {
            data.draworder = json.at("draworder").get<std::string>();
        }
        if (json.contains("id")) {
            data.id = json.at("id").get<int>();
        }
        if (json.contains("name")) {
            data.name = json.at("name").get<std::string>();
        }
        if (json.contains("objects")) {
            data.objects = json.at("objects").get<std::vector<TiledObject>>();
        }
        if (json.contains("opacity")) {
            data.opacity = json.at("opacity").get<float>();
        }
        if (json.contains("type")) {
            data.type = json.at("type").get<std::string>();
        }
        if (json.contains("visible")) {
            data.visible = json.at("visible").get<bool>();
        }
        if (json.contains("x")) {
            data.x = json.at("x").get<int>();
        }
        if (json.contains("y")) {
            data.y = json.at("y").get<int>();
        }
    }

    TiledObject TiledObject::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledObject>();
    }

    bool TiledObject::is_ellipse() const {
        return ellipse;
    }

    bool TiledObject::is_point() const {
        return point;
    }

    bool TiledObject::is_polygon() const {
        return polygons.size() > 0;
    }

    bool TiledObject::is_rectangle() const {
        return !is_ellipse() && !is_point() && !is_polygon();
    }

    const TiledProperty& TiledObject::get_property(const std::string& name) const {
        return get_tiled_property(properties, name);
    }

    const TiledProperty* TiledObject::try_get_property(const std::string& name) const {
        return try_get_tiled_property(properties, name);
    }
}
