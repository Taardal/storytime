#include "World.h"

namespace storytiled {

    const char* World::getTiledVersion() const {
        return tiledVersion;
    }

    void World::setTiledVersion(const char* tiledVersion) {
        World::tiledVersion = tiledVersion;
    }

    const char* World::getType() const {
        return type;
    }

    void World::setType(const char* type) {
        World::type = type;
    }

    int World::getWidth() const {
        return width;
    }

    void World::setWidth(int width) {
        World::width = width;
    }

    int World::getHeight() const {
        return height;
    }

    void World::setHeight(int height) {
        World::height = height;
    }

    int World::getTileWidth() const {
        return tileWidth;
    }

    void World::setTileWidth(int tileWidth) {
        World::tileWidth = tileWidth;
    }

    int World::getTileHeight() const {
        return tileHeight;
    }

    void World::setTileHeight(int tileHeight) {
        World::tileHeight = tileHeight;
    }

    bool World::isInfinite() const {
        return infinite;
    }

    void World::setInfinite(bool infinite) {
        World::infinite = infinite;
    }
}

