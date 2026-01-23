#include "st_tiled_tileset.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json&, TiledTile&);

    void from_json(const nlohmann::json& json, TiledTileset& data) {
        if (json.contains("columns")) {
            data.columns = json.at("columns").get<int>();
        }
        if (json.contains("firstgid")) {
            data.firstgid = json.at("firstgid").get<int>();
        }
        if (json.contains("image")) {
            data.image = json.at("image").get<std::string>();
        }
        if (json.contains("imageheight")) {
            data.imageheight = json.at("imageheight").get<int>();
        }
        if (json.contains("imagewidth")) {
            data.imagewidth = json.at("imagewidth").get<int>();
        }
        if (json.contains("margin")) {
            data.margin = json.at("margin").get<int>();
        }
        if (json.contains("name")) {
            data.name = json.at("name").get<std::string>();
        }
        if (json.contains("spacing")) {
            data.spacing = json.at("spacing").get<int>();
        }
        if (json.contains("tiles")) {
            data.tiles = json.at("tiles").get<std::vector<TiledTile>>();
        }
        if (json.contains("tilecount")) {
            data.tilecount = json.at("tilecount").get<int>();
        }
        if (json.contains("tileheight")) {
            data.tileheight = json.at("tileheight").get<int>();
        }
        if (json.contains("tilewidth")) {
            data.tilewidth = json.at("tilewidth").get<int>();
        }
    }

    TiledTileset TiledTileset::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledTileset>();
    }
}

namespace Storytime {
    void from_json(const nlohmann::json& json, TiledTilesetRef& data) {
        if (json.contains("firstgid")) {
            data.firstgid = json.at("firstgid").get<int>();
        }
        if (json.contains("source")) {
            data.source = json.at("source").get<std::string>();
        }
    }

    TiledTilesetRef TiledTilesetRef::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledTilesetRef>();
    }
}
