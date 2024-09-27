#include "tile.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json&, TiledObjectGroup&);

    void from_json(const nlohmann::json& json, TiledAnimationFrame& data) {
        data.duration = json.at("duration").get<int>();
        data.tileid = json.at("tileid").get<int>();
    }

    void from_json(const nlohmann::json& json, TiledTile& data) {
        if (json.contains("animation")) {
            data.animation = json.at("animation").get<std::vector<TiledAnimationFrame>>();
        }
        data.id = json.at("id").get<int>();
        if (json.contains("objectgroup")) {
            data.objectgroup = json.at("objectgroup").get<TiledObjectGroup>();
        }
    }

    TiledTile TiledTile::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledTile>();
    }
}
