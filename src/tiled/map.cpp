#include "map.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json& json, TiledTilesetRef& tileset_ref);

    extern void from_json(const nlohmann::json& json, TiledTileset& tileset);

    extern void from_json(const nlohmann::json& json, TiledLayer& layer);

    void from_json(const nlohmann::json& json, TiledMap& tiled_map) {
        tiled_map.height = json.at("height").get<int>();
        tiled_map.infinite = json.at("infinite").get<bool>();
        tiled_map.layers = json.at("layers").get<std::vector<TiledLayer>>();
        tiled_map.next_layer_id = json.at("nextlayerid").get<int>();
        tiled_map.next_object_id = json.at("nextobjectid").get<int>();
        tiled_map.orientation = json.at("orientation").get<std::string>();
        tiled_map.render_order = json.at("renderorder").get<std::string>();
        if (json.contains("tilesets")) {
            for (const nlohmann::json& tileset_json : json.at("tilesets")) {
                bool is_tileset_ref = tileset_json.contains("source");
                if (is_tileset_ref) {
                    tiled_map.tileset_refs.push_back(tileset_json.get<TiledTilesetRef>());
                } else {
                    tiled_map.tilesets.push_back(tileset_json.get<TiledTileset>());
                }
            }
        }
        tiled_map.tile_height = json.at("tileheight").get<int>();
        tiled_map.tile_width = json.at("tilewidth").get<int>();
        tiled_map.type = json.at("type").get<std::string>();
        tiled_map.version = json.at("version").get<std::string>();
        tiled_map.width = json.at("width").get<int>();
    }

    TiledMap TiledMap::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledMap>();
    }
}
