#include "st_tiled_template.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json&, TiledObject&);

    extern void from_json(const nlohmann::json&, TiledTilesetRef&);

    void from_json(const nlohmann::json& json, TiledObjectTemplate& data) {
        if (json.contains("object")) {
            data.object = json.at("object").get<TiledObject>();
        }
        if (json.contains("tileset")) {
            data.tileset = json.at("tileset").get<TiledTilesetRef>();
        }
        if (json.contains("type")) {
            data.type = json.at("type").get<std::string>();
        }
    }

    TiledObjectTemplate TiledObjectTemplate::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledObjectTemplate>();
    }
}
