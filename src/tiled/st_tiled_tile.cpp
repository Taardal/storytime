#include "st_tiled_tile.h"
#include <nlohmann/json.hpp>

namespace Storytime {
    extern void from_json(const nlohmann::json&, TiledObjectGroup&);

    extern void from_json(const nlohmann::json&, TiledProperty&);

    void from_json(const nlohmann::json& json, TiledAnimationFrame& data) {
        if (json.contains("duration")) {
            data.duration = json.at("duration").get<int>();
        }
        if (json.contains("tileid")) {
            data.tileid = json.at("tileid").get<int>();
        }
    }

    void from_json(const nlohmann::json& json, TiledTile& data) {
        if (json.contains("animation")) {
            data.animation = json.at("animation").get<std::vector<TiledAnimationFrame>>();
        }
        if (json.contains("id")) {
            data.id = json.at("id").get<int>();
        }
        if (json.contains("objectgroup")) {
            data.objectgroup = json.at("objectgroup").get<TiledObjectGroup>();
        }
        if (json.contains("properties")) {
            data.properties = json.at("properties").get<std::vector<TiledProperty>>();
        }
    }

    TiledTile TiledTile::create(const std::string& json) {
        return nlohmann::json::parse(json).get<TiledTile>();
    }

    const TiledProperty& TiledTile::get_property(const std::string& name) const {
        return get_tiled_property(properties, name);
    }

    const TiledProperty* TiledTile::try_get_property(const std::string& name) const {
        return try_get_tiled_property(properties, name);
    }

    const TiledObject* TiledTile::get_object(const std::string& name) const {
        for (const TiledObject& object : objectgroup.objects) {
            if (object.name == name) {
                return &object;
            }
        }
        return nullptr;
    }

    const TiledObject* TiledTile::get_object(i32 global_id) const {
        for (const TiledObject& object : objectgroup.objects) {
            if (object.gid == global_id) {
                return &object;
            }
        }
        return nullptr;
    }
}
