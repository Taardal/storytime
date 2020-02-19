#include "World.h"
#include "Tileset.h"
#include <nlohmann/json.hpp>
#include <iostream>

namespace storytiled {

    extern void from_json(const nlohmann::json& json, Tileset& tileset);

    void from_json(const nlohmann::json& json, World& world) {
        world.type = json.at("type").get<std::string>();
        world.width = json.at("width").get<int>();
        world.height = json.at("height").get<int>();
        world.tileWidth = json.at("tilewidth").get<int>();
        world.tileHeight = json.at("tileheight").get<int>();
        world.infinite = json.at("infinite").get<bool>();
        world.tilesets = json.at("tilesets").get<std::vector<Tileset>>();

        std::cout << json.at("tilesets") << std::endl;
        std::cout << "foo: " << json.at("tilesets")[0].contains("source") << std::endl;
        std::cout << "bar: " << json.at("tilesets")[0].contains("firstgid") << std::endl;
    }

    World World::fromJson(const std::string& json) {
        return nlohmann::json::parse(json).get<World>();
    }

}

