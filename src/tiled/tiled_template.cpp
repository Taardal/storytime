#include "tiled_template.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json&, TiledObject&);

    extern void from_json(const nlohmann::json&, TiledTilesetRef&);

    void from_json(const nlohmann::json& json, TiledObjectTemplate& data) {
        data.object = json.at("object").get<TiledObject>();
        data.tileset = json.at("tileset").get<TiledTilesetRef>();
        data.type = json.at("type").get<std::string>();
    }

    TiledObjectTemplate TiledObjectTemplate::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledObjectTemplate>();
    }
}

namespace Storytime {
    void from_json(const nlohmann::json& json, TiledObjectTemplateRef& data) {
        data.id = json.at("id").get<int>();
        data.template_path = json.at("template").get<std::string>();
        data.x = json.at("x").get<int>();
        data.y = json.at("y").get<int>();
    }

    TiledObjectTemplateRef TiledObjectTemplateRef::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledObjectTemplateRef>();
    }
}
