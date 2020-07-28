#include "Property.h"
#include <nlohmann/json.hpp>

namespace storytiled
{
    void from_json(const nlohmann::json& json, Property& property)
    {
        property.Name = json.at("name").get<std::string>();
        property.Type = json.at("type").get<std::string>();
        property.Value = json.at("value").get<std::string>();
    }
}

namespace storytiled
{
    Property::Property()
            : Name(), Type(), Value()
    {}
}
