#pragma once

#include "scene/scene.h"
#include "graphics/renderer.h"
#include "graphics/spritesheet.h"
#include "resource/resource_loader.h"

namespace Storytime {
    typedef u32 GlobalTileID;
    typedef u32 GlobalTiledID;

    struct TiledSceneConfig {
        FileSystem* file_system = nullptr;
        ResourceLoader* resource_loader = nullptr;
        Renderer* renderer = nullptr;
        std::filesystem::path map_path;
        f32 scale = 1.0f;
        bool debug = false;
    };

    class TiledScene : public Scene {
    protected:
        TiledSceneConfig config;
        Shared<TiledMap> tiled_map;
        std::map<GlobalTileID, Sprite> tiles;
        std::map<GlobalTiledID, TiledObject> tile_colliders;

    public:
        explicit TiledScene(const TiledSceneConfig& config);

        void on_initialize() override;

        void on_update(f64 timestep) override;

        void on_render() override;

    private:
        void initialize_tiles();

        void initialize_tiles(const TiledTileset& tileset, const std::filesystem::path& tileset_image_path);

        void initialize_tile_animation(const TiledTile& tiled_tile, u32 first_global_tile_id_in_tileset);

        void initialize_tile_collision(const TiledTile& tiled_tile, u32 first_global_tile_id_in_tileset);

        void update_tiles(f64 timestep);

        void render_tile_layer(const TiledLayer& layer) const;
    };
}