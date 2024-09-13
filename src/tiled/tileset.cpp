#include "tileset.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    void from_json(const nlohmann::json& json, TiledTileset& tileset) {
        tileset.columns = json.at("columns").get<int>();
        if (json.contains("firstgid")) {
            tileset.firstgid = json.at("firstgid").get<int>();
        }
        tileset.image = json.at("image").get<std::string>();
        tileset.image_height = json.at("imageheight").get<int>();
        tileset.image_width = json.at("imagewidth").get<int>();
        tileset.margin = json.at("margin").get<int>();
        tileset.name = json.at("name").get<std::string>();
        tileset.spacing = json.at("spacing").get<int>();
        tileset.tile_count = json.at("tilecount").get<int>();
        tileset.tile_height = json.at("tileheight").get<int>();
        tileset.tile_width = json.at("tilewidth").get<int>();
    }

    TiledTileset TiledTileset::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledTileset>();
    }
}

namespace Storytime {
    void from_json(const nlohmann::json& json, TiledTilesetRef& tileset_ref) {
        tileset_ref.firstgid = json.at("firstgid").get<int>();
        tileset_ref.source = json.at("source").get<std::string>();
    }

    TiledTilesetRef TiledTilesetRef::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledTilesetRef>();
    }
}
