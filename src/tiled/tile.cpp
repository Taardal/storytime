#include "tile.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    void from_json(const nlohmann::json& json, TiledAnimationFrame& frame) {
        frame.duration = json.at("duration").get<int>();
        frame.tileid = json.at("tileid").get<int>();
    }

    void from_json(const nlohmann::json& json, TiledTile& tile) {
        tile.id = json.at("id").get<int>();
        tile.animation = json.at("animation").get<std::vector<TiledAnimationFrame>>();
    }

    TiledTile TiledTile::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledTile>();
    }
}
