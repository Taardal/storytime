#include "tileset.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledTile& tile);

    void from_json(const nlohmann::json& json, TiledTileset& tileset) {
        tileset.columns = json.at("columns").get<int>();
        if (json.contains("firstgid")) {
            tileset.firstgid = json.at("firstgid").get<int>();
        }
        tileset.image = json.at("image").get<std::string>();
        tileset.imageheight = json.at("imageheight").get<int>();
        tileset.imagewidth = json.at("imagewidth").get<int>();
        tileset.margin = json.at("margin").get<int>();
        tileset.name = json.at("name").get<std::string>();
        tileset.spacing = json.at("spacing").get<int>();
        if (json.contains("tiles")) {
            tileset.tiles = json.at("tiles").get<std::vector<TiledTile>>();
        }
        tileset.tilecount = json.at("tilecount").get<int>();
        tileset.tileheight = json.at("tileheight").get<int>();
        tileset.tilewidth = json.at("tilewidth").get<int>();
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
