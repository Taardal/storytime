#pragma once

#include "st_tiled_tileset.h"
#include "st_tiled_layer.h"
#include "st_tiled_property.h"

namespace Storytime {
    struct TiledMap {
        int height;
        bool infinite;
        std::vector<TiledLayer> layers;
        int nextlayerid;
        int nextobjectid;
        std::string orientation;
        std::vector<TiledProperty> properties;
        std::string renderorder;
        std::vector<TiledTileset> tilesets;
        std::vector<TiledTilesetRef> tilesetrefs;
        int tileheight;
        int tilewidth;
        std::string type;
        std::string version;
        int width;

        static TiledMap create(const std::string& json);

        const TiledProperty& get_property(const std::string& name) const;

        const TiledProperty* try_get_property(const std::string& name) const;

        template<typename T>
        T get_property_value(const std::string& name) {
            return get_tiled_property_value<T>(properties, name);
        }

        template<typename T>
        std::optional<T> try_get_property_value(const std::string& name) {
            return try_get_tiled_property_value<T>(properties, name);
        }
    };
}
