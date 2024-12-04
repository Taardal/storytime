#include "tiled_map.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json&, TiledTilesetRef&);

    extern void from_json(const nlohmann::json&, TiledTileset&);

    extern void from_json(const nlohmann::json&, TiledLayer&);

    void from_json(const nlohmann::json& json, TiledMap& data) {
        data.height = json.at("height").get<int>();
        data.infinite = json.at("infinite").get<bool>();
        data.layers = json.at("layers").get<std::vector<TiledLayer>>();
        data.nextlayerid = json.at("nextlayerid").get<int>();
        data.nextobjectid = json.at("nextobjectid").get<int>();
        data.orientation = json.at("orientation").get<std::string>();
        data.renderorder = json.at("renderorder").get<std::string>();
        if (json.contains("tilesets")) {
            for (const nlohmann::json& tileset_json : json.at("tilesets")) {
                bool is_tileset_ref = tileset_json.contains("source");
                if (is_tileset_ref) {
                    data.tilesetrefs.push_back(tileset_json.get<TiledTilesetRef>());
                } else {
                    data.tilesets.push_back(tileset_json.get<TiledTileset>());
                }
            }
        }
        data.tileheight = json.at("tileheight").get<int>();
        data.tilewidth = json.at("tilewidth").get<int>();
        data.type = json.at("type").get<std::string>();
        data.version = json.at("version").get<std::string>();
        data.width = json.at("width").get<int>();
    }

    TiledMap TiledMap::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledMap>();
    }
}
