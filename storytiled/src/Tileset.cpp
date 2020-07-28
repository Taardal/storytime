#include "Tileset.h"
#include <nlohmann/json.hpp>

namespace storytiled
{
    void from_json(const nlohmann::json& json, Tileset& tileset)
    {
        tileset.Name = json.at("name").get<std::string>();
        tileset.Image = json.at("image").get<std::string>();
        tileset.FirstGlobalId = json.at("firstgid").get<int>();
        tileset.ImageWidth = json.at("imagewidth").get<int>();
        tileset.ImageHeight = json.at("imageheight").get<int>();
        tileset.TileWidth = json.at("tilewidth").get<int>();
        tileset.TileHeight = json.at("tileheight").get<int>();
        tileset.TileCount = json.at("tilecount").get<int>();
        tileset.Columns = json.at("columns").get<int>();
        tileset.Margin = json.at("margin").get<int>();
        tileset.Spacing = json.at("spacing").get<int>();
    }
}

namespace storytiled
{
    Tileset::Tileset()
            : Name(),
              Image(),
              FirstGlobalId(0),
              ImageWidth(0),
              ImageHeight(0),
              TileWidth(0),
              TileHeight(0),
              TileCount(0),
              Columns(0),
              Margin(0),
              Spacing(0)
    {}
}

