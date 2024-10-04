#pragma once

#include "object.h"
#include "tileset.h"

namespace Storytime {
    struct TiledObjectTemplate {
        TiledObject object;
        TiledTilesetRef tileset;
        std::string type;

        static TiledObjectTemplate create(const std::string& json);
    };

    struct TiledObjectTemplateRef {
        int id;
        std::string template_path;
        int x;
        int y;

        static TiledObjectTemplateRef create(const std::string& json);
    };
}
