#include "tile.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledObject& object);

    void from_json(const nlohmann::json& json, TiledAnimationFrame& frame) {
        frame.duration = json.at("duration").get<int>();
        frame.tileid = json.at("tileid").get<int>();
    }

    void from_json(const nlohmann::json& json, TiledObjectGroup& object_group) {
        object_group.draworder = json.at("draworder").get<std::string>();
        object_group.id = json.at("id").get<int>();
        object_group.name = json.at("name").get<std::string>();
        object_group.objects = json.at("objects").get<std::vector<TiledObject>>();
        object_group.opacity = json.at("opacity").get<float>();
        object_group.type = json.at("type").get<std::string>();
        object_group.visible = json.at("visible").get<bool>();
        object_group.x = json.at("x").get<int>();
        object_group.y = json.at("y").get<int>();
    }

    void from_json(const nlohmann::json& json, TiledTile& tile) {
        tile.animation = json.at("animation").get<std::vector<TiledAnimationFrame>>();
        tile.id = json.at("id").get<int>();
        if (json.contains("objectgroup")) {
            tile.objectgroup = json.at("objectgroup").get<TiledObjectGroup>();
        }
    }

    TiledTile TiledTile::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledTile>();
    }
}
