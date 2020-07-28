#include "World.h"
#include <nlohmann/json.hpp>

namespace storytiled
{
    extern void from_json(const nlohmann::json& json, Tileset& tileset);

    extern void from_json(const nlohmann::json& json, Layer& layer);

    void from_json(const nlohmann::json& json, World& world)
    {
        world.Tilesets = json.at("tilesets").get<std::vector<Tileset>>();
        world.Layers = json.at("layers").get<std::vector<Layer>>();
        world.Type = json.at("type").get<std::string>();
        world.Orientation = json.at("orientation").get<std::string>();
        world.RenderOrder = json.at("renderorder").get<std::string>();
        world.Version = json.at("version").get<float>();
        world.Width = json.at("width").get<int>();
        world.Height = json.at("height").get<int>();
        world.TileWidth = json.at("tilewidth").get<int>();
        world.TileHeight = json.at("tileheight").get<int>();
        world.NextLayerId = json.at("nextlayerid").get<int>();
        world.NextObjectId = json.at("nextobjectid").get<int>();
        world.Infinite = json.at("infinite").get<bool>();
    }
}

namespace storytiled
{
    World::World()
            : Tilesets(),
              Layers(),
              Type(),
              Orientation(),
              RenderOrder(),
              Version(0.0f),
              Width(0),
              Height(0),
              TileWidth(0),
              TileHeight(0),
              NextLayerId(0),
              NextObjectId(0),
              Infinite(false)
    {}

    World World::FromJson(const std::string& json)
    {
        return nlohmann::json::parse(json).get<World>();
    }
}

