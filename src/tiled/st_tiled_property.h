#pragma once

#include <nlohmann/json.hpp>
#include <any>

namespace Storytime {
    using TiledPropertyValue = std::variant<bool, int, float, std::string, nlohmann::json>;

    struct TiledProperty {
        std::string name;
        std::string propertytype;
        std::string type;
        TiledPropertyValue value;

        template<typename T>
        T get_value() const {
            ST_ASSERT(std::holds_alternative<T>(value), "Property [" << name << "] must hold [" << type_name<T>() << "]");
            return std::get<T>(value);
        }
    };

    const TiledProperty& get_tiled_property(const std::vector<TiledProperty>& properties, const std::string& name);

    const TiledProperty* try_get_tiled_property(const std::vector<TiledProperty>& properties, const std::string& name);

    template<typename T>
    T get_tiled_property_value(const std::vector<TiledProperty>& properties, const std::string& name) {
        for (const TiledProperty& property : properties) {
            if (property.name == name) {
                ST_ASSERT(std::holds_alternative<T>(property.value), "Property [" << name << "] must hold [" << type_name<T>() << "]");
                return std::get<T>(property.value);
            }
        }
        ST_THROW("Could not find property [" << name << "] to get value of type [" << type_name<T>() << "]");
    }

    template<typename T>
    std::optional<T> try_get_tiled_property_value(const std::vector<TiledProperty>& properties, const std::string& name) {
        for (const TiledProperty& property : properties) {
            if (property.name == name) {
                ST_ASSERT(std::holds_alternative<T>(property.value), "Property [" << name << "] must hold [" << type_name<T>() << "]");
                return std::get<T>(property.value);
            }
        }
        return std::nullopt;
    }
}
