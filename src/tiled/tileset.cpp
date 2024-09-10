#include "tileset.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    void from_json(const nlohmann::json& json, Tileset& tileset) {
        tileset.name = json.at("name").get<std::string>();
        tileset.image = json.at("image").get<std::string>();
        tileset.first_global_id = json.at("firstgid").get<int>();
        tileset.image_width = json.at("imagewidth").get<int>();
        tileset.image_height = json.at("imageheight").get<int>();
        tileset.tile_width = json.at("tilewidth").get<int>();
        tileset.tile_height = json.at("tileheight").get<int>();
        tileset.tile_count = json.at("tilecount").get<int>();
        tileset.columns = json.at("columns").get<int>();
        tileset.margin = json.at("margin").get<int>();
        tileset.spacing = json.at("spacing").get<int>();
    }
}