#include "world.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, Tileset& tileset);

    extern void from_json(const nlohmann::json& json, Layer& layer);

    void from_json(const nlohmann::json& json, World& world) {
        // world.tilesets = json.at("tilesets").get<std::vector<Tileset>>();
        // world.layers = json.at("layers").get<std::vector<Layer>>();
        world.type = json.at("type").get<std::string>();
        world.orientation = json.at("orientation").get<std::string>();
        world.render_order = json.at("renderorder").get<std::string>();
        world.version = json.at("version").get<std::string>();
        world.width = json.at("width").get<int>();
        world.height = json.at("height").get<int>();
        world.tile_width = json.at("tilewidth").get<int>();
        world.tile_height = json.at("tileheight").get<int>();
        world.next_layer_id = json.at("nextlayerid").get<int>();
        world.next_object_id = json.at("nextobjectid").get<int>();
        world.infinite = json.at("infinite").get<bool>();
    }

    World World::create(const std::string& json) {
        return nlohmann::json::parse(json).get<World>();
    }
}