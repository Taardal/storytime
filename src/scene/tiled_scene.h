#pragma once

#include "graphics/spritesheet.h"
#include "resource/resource_loader.h"

namespace Storytime {
    typedef u32 TileID;

    struct TiledSceneConfig {
        FileSystem* file_system = nullptr;
        ResourceLoader* resource_loader = nullptr;
        std::filesystem::path tiled_map_path;
    };

    class TiledScene {
    private:
        static const f32 SCALE;
        static const bool DEBUG;

    private:
        TiledSceneConfig config;
        Shared<TiledMap> tiled_map;
        std::map<TileID, Sprite> tiles;

    public:
        explicit TiledScene(const TiledSceneConfig& config);

        void update(f64 timestep);

        void render(Renderer& renderer) const;

    private:
        void initialize_tiles();

        void initialize_tiles(const TiledTileset& tileset, const std::filesystem::path& tileset_image_path);

        void initialize_tile_animation(const TiledTile& tiled_tile, u32 first_global_tile_id_in_tileset);

        void initialize_tile_collision(const TiledTile& tiled_tile, u32 first_global_tile_id_in_tileset);

        void render_tile_layer(Renderer& renderer, const TiledLayer& layer) const;

        void render_object_layer(Renderer& renderer, const TiledLayer& layer) const;
    };
}