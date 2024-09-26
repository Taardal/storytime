#include "template.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledObject& object);

    extern void from_json(const nlohmann::json& json, TiledTilesetRef& tileset_ref);

    void from_json(const nlohmann::json& json, TiledObjectTemplate& object_template) {
        object_template.object = json.at("object").get<TiledObject>();
        object_template.tileset = json.at("tileset").get<TiledTilesetRef>();
        object_template.type = json.at("type").get<std::string>();
    }

    TiledObjectTemplate TiledObjectTemplate::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledObjectTemplate>();
    }
}

namespace Storytime {
    void from_json(const nlohmann::json& json, TiledObjectTemplateRef& object_template_ref) {
        object_template_ref.id = json.at("id").get<int>();
        object_template_ref.template_path = json.at("template").get<std::string>();
        object_template_ref.x = json.at("x").get<int>();
        object_template_ref.y = json.at("y").get<int>();
    }

    TiledObjectTemplateRef TiledObjectTemplateRef::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledObjectTemplateRef>();
    }
}
