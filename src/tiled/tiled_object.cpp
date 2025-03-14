#include "tiled_object.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledProperty& data);

    void from_json(const nlohmann::json& json, TiledObject& object) {
        if (json.contains("gid")) {
            object.gid = json.at("gid").get<int>();
        }
        object.height = json.at("height").get<int>();
        object.id = json.at("id").get<int>();
        object.name = json.at("name").get<std::string>();
        if (json.contains("point")) {
            object.point = json.at("point").get<bool>();
        }
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

    TiledObject TiledObject::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledObject>();
    }

    const TiledProperty& TiledObject::get_property(const std::string& name) const {
        return get_tiled_property(properties, name);
    }

    const TiledProperty* TiledObject::try_get_property(const std::string& name) const {
        return try_get_tiled_property(properties, name);
    }

    void from_json(const nlohmann::json& json, TiledObjectGroup& object_group) {
        object_group.draworder = json.at("draworder").get<std::string>();
        if (json.contains("id")) {
            object_group.id = json.at("id").get<int>();
        }
        object_group.name = json.at("name").get<std::string>();
        object_group.objects = json.at("objects").get<std::vector<TiledObject>>();
        object_group.opacity = json.at("opacity").get<float>();
        object_group.type = json.at("type").get<std::string>();
        object_group.visible = json.at("visible").get<bool>();
        object_group.x = json.at("x").get<int>();
        object_group.y = json.at("y").get<int>();
    }
}
