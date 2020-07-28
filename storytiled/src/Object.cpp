#include "Object.h"
#include <nlohmann/json.hpp>

namespace storytiled
{
    extern void from_json(const nlohmann::json& json, Property& property);

    void from_json(const nlohmann::json& json, Object& object)
    {
        object.Name = json.at("name").get<std::string>();
        object.Type = json.at("type").get<std::string>();
        object.Rotation = json.at("rotation").get<float>();
        object.Id = json.at("id").get<int>();
        object.X = json.at("x").get<int>();
        object.Y = json.at("y").get<int>();
        object.Width = json.at("width").get<int>();
        object.Height = json.at("height").get<int>();
        object.Visible = json.at("visible").get<bool>();
        if (json.contains("properties"))
        {
            object.Properties = json.at("properties").get<std::vector<Property>>();
        }
    }
}

namespace storytiled
{
    Object::Object()
            : Properties(),
              Name(),
              Type(),
              Rotation(0.0f),
              Id(0),
              X(0),
              Y(0),
              Width(0),
              Height(0),
              Visible(false)
    {}
}
