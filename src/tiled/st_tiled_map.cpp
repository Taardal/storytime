#include "st_tiled_map.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json&, TiledTilesetRef&);

    extern void from_json(const nlohmann::json&, TiledTileset&);

    extern void from_json(const nlohmann::json&, TiledLayer&);

    extern void from_json(const nlohmann::json&, TiledProperty&);

    void from_json(const nlohmann::json& json, TiledMap& data) {
        if (json.contains("height")) {
            data.height = json.at("height").get<int>();
        }
        if (json.contains("infinite")) {
            data.infinite = json.at("infinite").get<bool>();
        }
        if (json.contains("layers")) {
            data.layers = json.at("layers").get<std::vector<TiledLayer>>();
        }
        if (json.contains("nextlayerid")) {
            data.nextlayerid = json.at("nextlayerid").get<int>();
        }
        if (json.contains("nextobjectid")) {
            data.nextobjectid = json.at("nextobjectid").get<int>();
        }
        if (json.contains("orientation")) {
            data.orientation = json.at("orientation").get<std::string>();
        }
        if (json.contains("properties")) {
            data.properties = json.at("properties").get<std::vector<TiledProperty>>();
        }
        if (json.contains("renderorder")) {
            data.renderorder = json.at("renderorder").get<std::string>();
        }
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
        if (json.contains("tileheight")) {
            data.tileheight = json.at("tileheight").get<int>();
        }
        if (json.contains("tilewidth")) {
            data.tilewidth = json.at("tilewidth").get<int>();
        }
        if (json.contains("type")) {
            data.type = json.at("type").get<std::string>();
        }
        if (json.contains("version")) {
            data.version = json.at("version").get<std::string>();
        }
        if (json.contains("width")) {
            data.width = json.at("width").get<int>();
        }
    }

    TiledMap TiledMap::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledMap>();
    }

    const TiledProperty& TiledMap::get_property(const std::string& name) const {
        return get_tiled_property(properties, name);
    }

    const TiledProperty* TiledMap::try_get_property(const std::string& name) const {
        return try_get_tiled_property(properties, name);
    }
}
