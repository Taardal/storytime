#pragma once

#include "scene/scene.h"
#include "graphics/spritesheet.h"
#include "resource/resource_loader.h"

namespace Storytime {
    typedef u32 GlobalTileID;
    typedef u32 GlobalTiledID;

    struct TiledSceneConfig {
        FileSystem* file_system = nullptr;
        ResourceLoader* resource_loader = nullptr;
        std::filesystem::path tiled_map_path;
    };

    class TiledScene : public Scene {
    private:
        static const f32 SCALE;
        static const bool DEBUG;

    private:
        TiledSceneConfig config;
        Shared<TiledMap> tiled_map;
        std::map<GlobalTileID, Sprite> tiles;
        std::map<GlobalTiledID, TiledObjectTemplate> object_templates;

    public:
        explicit TiledScene(const TiledSceneConfig& config);

        void on_initialize() override;

        void on_update(f64 timestep) override;

        void on_render(Renderer& renderer) override;

    protected:
        void on_create_entities() override;

        virtual void on_add_entity_components(
            entt::entity entity,
            const TiledObject& tiled_object,
            const std::filesystem::path& tiled_map_directory_path
        ) {}

    private:
        void initialize_tiles();

        void initialize_tiles(const TiledTileset& tileset, const std::filesystem::path& tileset_image_path);

        void initialize_tile_animation(const TiledTile& tiled_tile, u32 first_global_tile_id_in_tileset);

        void initialize_tile_collision(const TiledTile& tiled_tile, u32 first_global_tile_id_in_tileset);

        void initialize_objects();

        void initialize_object_templates();

        void render_tile_layer(Renderer& renderer, const TiledLayer& layer) const;

        void render_object_layer(Renderer& renderer, const TiledLayer& layer) const;
    };
}