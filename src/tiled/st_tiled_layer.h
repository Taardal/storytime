#pragma once

#include "st_tiled_object.h"
#include "st_tiled_property.h"

namespace Storytime {
    struct TiledLayer {
        static constexpr float default_parallax = 1.0f;

        std::vector<int> data{};
        std::string draworder = "";
        int height = 0;
        int id = 0;
        std::string image = "";
        std::vector<TiledLayer> layers{};
        bool locked = false;
        std::string name = "";
        float offsetx = 0.0f;
        float offsety = 0.0f;
        std::vector<TiledObject> objects{};
        int opacity = 0;
        float parallaxx = default_parallax;
        float parallaxy = default_parallax;
        std::vector<TiledProperty> properties{};
        std::string type = "";
        bool visible = false;
        int width = 0;
        float x = 0.0f;
        float y = 0.0f;

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

        bool is_parallax() const;
    };
}
