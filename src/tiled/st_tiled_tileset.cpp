#include "st_tiled_tileset.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json&, TiledTile&);

    void from_json(const nlohmann::json& json, TiledTileset& data) {
        data.columns = json.at("columns").get<int>();
        if (json.contains("firstgid")) {
            data.firstgid = json.at("firstgid").get<int>();
        }
        data.image = json.at("image").get<std::string>();
        data.imageheight = json.at("imageheight").get<int>();
        data.imagewidth = json.at("imagewidth").get<int>();
        data.margin = json.at("margin").get<int>();
        data.name = json.at("name").get<std::string>();
        data.spacing = json.at("spacing").get<int>();
        if (json.contains("tiles")) {
            data.tiles = json.at("tiles").get<std::vector<TiledTile>>();
        }
        data.tilecount = json.at("tilecount").get<int>();
        data.tileheight = json.at("tileheight").get<int>();
        data.tilewidth = json.at("tilewidth").get<int>();
    }

    TiledTileset TiledTileset::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledTileset>();
    }
}

namespace Storytime {
    void from_json(const nlohmann::json& json, TiledTilesetRef& data) {
        data.firstgid = json.at("firstgid").get<int>();
        data.source = json.at("source").get<std::string>();
    }

    TiledTilesetRef TiledTilesetRef::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledTilesetRef>();
    }
}
