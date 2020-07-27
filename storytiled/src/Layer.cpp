#include "Layer.h"
#include <nlohmann/json.hpp>

namespace storytiled
{
    extern void from_json(const nlohmann::json& json, Property& property);

    extern void from_json(const nlohmann::json&, Object& object);

    void from_json(const nlohmann::json& json, Layer& layer)
    {
        layer.Name = json.at("name").get<std::string>();
        layer.Type = json.at("type").get<std::string>();
        layer.Id = json.at("id").get<int>();
        layer.X = json.at("x").get<int>();
        layer.Y = json.at("y").get<int>();
        layer.Opacity = json.at("opacity").get<int>();
        layer.Visible = json.at("visible").get<bool>();
        if (layer.Type == "objectgroup")
        {
            layer.Objects = json.at("objects").get<std::vector<Object>>();
            layer.DrawOrder = json.at("draworder").get<std::string>();
        }
        else if (layer.Type == "tilelayer")
        {
            layer.Data = json.at("data").get<std::vector<int>>();
            layer.Width = json.at("width").get<int>();
            layer.Height = json.at("height").get<int>();
        }
        else if (layer.Type == "image")
        {
            layer.Image = json.at("image").get<std::string>();
        }
        else if (layer.Type == "group")
        {
            layer.Layers = json.at("layers").get<std::vector<Layer>>();
        }
        if (json.contains("properties"))
        {
            layer.Properties = json.at("properties").get<std::vector<Property>>();
        }
    }
}

namespace storytiled
{
    Layer::Layer()
            : Layers(),
              Objects(),
              Data(),
              Name(),
              Type(),
              DrawOrder(),
              Image(),
              Id(0),
              X(0),
              Y(0),
              Width(0),
              Height(0),
              Opacity(0),
              Visible(false)
    {}
}
