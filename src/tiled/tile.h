#pragma once

namespace Storytime {
    struct TiledAnimationFrame {
        int duration;
        int tileid;
    };

    struct TiledTile {
        std::vector<TiledAnimationFrame> animation;
        int id;

        static TiledTile create(const std::string& json);
    };
}