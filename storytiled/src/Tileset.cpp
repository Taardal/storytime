#include "Tileset.h"
#include <nlohmann/json.hpp>

namespace storytiled {

    void from_json(const nlohmann::json& json, Tileset& tileset) {
        tileset.firstGlobalId = json.at("firstgid").get<int>();
        tileset.name = json.at("name").get<std::string>();
        tileset.imagePath = json.at("image").get<std::string>();
        tileset.imageWidth = json.at("imagewidth").get<int>();
        tileset.imageHeight = json.at("imageheight").get<int>();
        tileset.tileWidth = json.at("tilewidth").get<int>();
        tileset.tileHeight = json.at("tileheight").get<int>();
        tileset.numberOfTiles = json.at("tilecount").get<int>();
        tileset.numberOfColumns = json.at("columns").get<int>();
        tileset.margin = json.at("margin").get<int>();
        tileset.spacing = json.at("spacing").get<int>();
    }

}

