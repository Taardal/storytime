#pragma once

#include "st_tiled_object.h"
#include "st_tiled_property.h"

namespace Storytime {
    struct TiledAnimationFrame {
        int duration;
        int tileid;
    };

    struct TiledTile {
        std::vector<TiledAnimationFrame> animation;
        int id;
        TiledObjectGroup objectgroup;
        std::vector<TiledProperty> properties;

        static TiledTile create(const std::string& json);

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

        const TiledObject* get_object(const std::string& name) const;

        const TiledObject* get_object(i32 global_id) const;
    };
}
