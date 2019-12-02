#include "World.h"
#include <nlohmann/json.hpp>

namespace storytiled {

    World World::fromJson(const std::string& json) {
        auto jsonObject = nlohmann::json::parse(json);
        World world;
        world.tiledVersion = jsonObject["tiledversion"].get<std::string>();
        world.type = jsonObject["type"].get<std::string>();
        world.width = jsonObject["width"].get<int>();
        world.height = jsonObject["height"].get<int>();
        world.tileWidth = jsonObject["tilewidth"].get<int>();
        world.tileHeight = jsonObject["tileheight"].get<int>();
        world.infinite = jsonObject["infinite"].get<bool>();
        return world;
    }

}

