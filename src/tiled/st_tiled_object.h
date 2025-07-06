#pragma once

#include "st_tiled_property.h"

namespace Storytime {
    struct TiledObject {
        bool ellipse;
        int gid;
        int height;
        int id;
        std::string name;
        bool point;
        std::vector<glm::vec2> polygons;
        std::vector<TiledProperty> properties;
        float rotation;
        std::string template_path;
        std::string type;
        bool visible;
        int width;
        float x;
        float y;

        static TiledObject create(const std::string& json);

        bool is_ellipse() const;

        bool is_point() const;

        bool is_polygon() const;

        bool is_rectangle() const;

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

    struct TiledObjectGroup {
        std::string draworder;
        int id;
        std::string name;
        std::vector<TiledObject> objects;
        float opacity;
        std::string type;
        bool visible;
        int x;
        int y;
    };
}
