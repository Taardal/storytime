#pragma once

#include "st_tiled_object.h"
#include "st_tiled_property.h"
#include "st_tiled_template.h"

namespace Storytime {
    struct TiledLayer {
        std::vector<TiledProperty> properties;
        std::vector<TiledLayer> layers;
        std::vector<TiledObject> objects;
        std::vector<TiledObjectTemplateRef> object_template_refs;
        std::vector<int> data;
        std::string name;
        std::string type;
        std::string draworder;
        std::string image;
        int id;
        int x;
        int y;
        int width;
        int height;
        int opacity;
        bool visible;

        static TiledLayer create(const std::string& json);

        const TiledProperty& get_property(const std::string& name) const;

        const TiledProperty* try_get_property(const std::string& name) const;

        template<typename T>
        T get_property_value(const std::string& name) const {
            return get_tiled_property_value<T>(properties, name);
        }

        template<typename T>
        std::optional<T> try_get_property_value(const std::string& name) const {
            return try_get_tiled_property_value<T>(properties, name);
        }
    };
}
