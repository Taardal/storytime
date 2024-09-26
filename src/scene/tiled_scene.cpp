#include "tiled_scene.h"
#include "components.h"

namespace Storytime {
    constexpr f32 TiledScene::SCALE = 1.0f;
    constexpr bool TiledScene::DEBUG = false;

    TiledScene::TiledScene(const TiledSceneConfig& config)
        : config(config), tiled_map(config.resource_loader->load_tiled_map(config.tiled_map_path.c_str())) {
    }

    void TiledScene::on_initialize() {
        initialize_tiles();
        initialize_objects();
    }

    void TiledScene::on_update(f64 timestep) {
        for (auto& [global_tile_id, tile] : tiles) {
            if (!tile.is_animated()) {
                continue;
            }
            tile.update(timestep);
        }
        for (entt::entity entity : entity_registry.view<TransformComponent>()) {

        }
    }

    void TiledScene::on_render(Renderer& renderer) {
        for (const TiledLayer& layer : tiled_map->layers) {
            if (!layer.visible) {
                continue;
            }
            if (layer.type == "tilelayer") {
                render_tile_layer(renderer, layer);
            } else if (layer.type == "objectgroup") {
                render_object_layer(renderer, layer);
            }
        }
        for (entt::entity entity : entity_registry.view<TransformComponent, SpriteComponent>()) {
            auto& transform_component = entity_registry.get<TransformComponent>(entity);
            auto& sprite_component = entity_registry.get<SpriteComponent>(entity);

            Sprite::RenderConfig render_config{
                .position = transform_component.position,
                .debug = true,
            };

            sprite_component.sprite.render(renderer, render_config);
        }
    }

    void TiledScene::on_create_entities() {
        std::filesystem::path tiled_map_directory_path = config.tiled_map_path.parent_path();

        for (const TiledLayer& layer : tiled_map->layers) {
            if (!layer.visible || layer.type != "objectgroup") {
                continue;
            }
            for (const TiledObject& tiled_object : layer.objects) {
                entt::entity entity = entity_registry.create();

                TagComponent tag_component{};
                tag_component.tag = tiled_object.name.size() > 0 ? tiled_object.name : "Tiled Object";
                entity_registry.emplace<TagComponent>(entity, tag_component);

                TransformComponent transform_component{};
                transform_component.position = glm::vec3(tiled_object.x, tiled_object.y, 0.0f);
                transform_component.size = glm::vec3((f32) tiled_object.width, (f32) tiled_object.height, 1.0f);
                entity_registry.emplace<TransformComponent>(entity, transform_component);

                on_add_entity_components(entity, tiled_object, tiled_map_directory_path);
            }
        }
    }

    void TiledScene::initialize_objects() {
        on_create_entities();
    }

    void TiledScene::initialize_tiles() {
        // Tileset paths are relative to the map that is using them
        auto tiled_map_directory_path = config.tiled_map_path.parent_path();

        // Tileset refs
        for (const TiledTilesetRef& tileset_ref : tiled_map->tilesetrefs) {
            // Get the path to the tileset json file relative to the map
            auto tileset_path = std::filesystem::path(tiled_map_directory_path).append(tileset_ref.source);

            std::string tileset_json = config.file_system->read_file(tileset_path.c_str());
            ST_ASSERT_THROW(!tileset_json.empty());

            auto tileset = TiledTileset::create(tileset_json);
            tileset.firstgid = tileset_ref.firstgid;

            auto tileset_image_path = std::filesystem::path(tileset_path.parent_path()).append(tileset.image);
            initialize_tiles(tileset, tileset_image_path);
        }

        // Tilesets
        for (const TiledTileset& tileset : tiled_map->tilesets) {
            // Get the path to the tileset image file relative to the map
            auto tileset_image_path = std::filesystem::path(tiled_map_directory_path).append(tileset.image);
            initialize_tiles(tileset, tileset_image_path);
        }
    }

    void TiledScene::initialize_tiles(
        const TiledTileset& tileset,
        const std::filesystem::path& tileset_image_path
    ) {
        // Load the tileset image
        Shared<Texture> tileset_image_texture = config.resource_loader->load_texture(tileset_image_path.c_str());

        // Iterate through the tiles of the tileset as a grid
        u32 rows = tileset.imageheight / tileset.tileheight;
        u32 columns = tileset.imagewidth / tileset.tilewidth;
        u32 first_global_tile_id_in_tileset = tileset.firstgid;

        // Create all tiles in tileset as static sprites (single image) and associate it to its global tile ID
        for (u32 row = 0; row < rows; ++row) {
            for (u32 column = 0; column < columns; ++column) {
                u32 tile_index = row * columns + column;
                u32 global_tile_id = first_global_tile_id_in_tileset + tile_index;

                SpriteConfig tile_config;
                tile_config.spritesheet_texture = tileset_image_texture;
                tile_config.width = tileset.tilewidth;
                tile_config.height = tileset.tileheight;
                tile_config.spritesheet_coordinate = SpritesheetCoordinate{
                    .column = column,
                    .row = row
                };

                tiles.emplace(global_tile_id, tile_config);
            }
        }

        // Go through all tiles in tileset and initialize both animated tiles and collidable tiles
        for (const TiledTile& tiled_tile : tileset.tiles) {
            const std::vector<TiledAnimationFrame>& animation_frames = tiled_tile.animation;
            if (animation_frames.size() > 0) {
                initialize_tile_animation(tiled_tile, first_global_tile_id_in_tileset);
            }
            if (tiled_tile.objectgroup.objects.size() > 0) {
                initialize_tile_collision(tiled_tile, first_global_tile_id_in_tileset);
            }
        }
    }

    // Update static sprite to be animated sprite by updating its spritesheet coordinates to form an animation sequence.
    // This must be done after all tiles has been created because we need to be able to look up all static sprites
    // needed for the animation sequence.
    void TiledScene::initialize_tile_animation(const TiledTile& tiled_tile, u32 first_global_tile_id_in_tileset) {
        // Find static sprite that will be updated to be animated
        u32 global_tile_id = first_global_tile_id_in_tileset + tiled_tile.id;
        auto tile_iterator = tiles.find(global_tile_id);
        ST_ASSERT_THROW(tile_iterator != tiles.end());
        Sprite& tile = tile_iterator->second;

        // Get spritesheet coordinates for all sprites in animation
        std::vector<SpritesheetCoordinate> animation_spritesheet_coordinates;
        for (auto& animation_frame : tiled_tile.animation) {

            // Find static sprite for the current frame of the animation
            u32 animation_frame_tile_global_tile_id = first_global_tile_id_in_tileset + animation_frame.tileid;
            auto animation_frame_tile_iterator = tiles.find(animation_frame_tile_global_tile_id);
            ST_ASSERT_THROW(animation_frame_tile_iterator != tiles.end());
            Sprite& animation_frame_tile = animation_frame_tile_iterator->second;

            // Get the spritesheet coordinate of the static sprite and add it to the list of spritesheet
            // coordinates that makes the animation sequence.
            auto [row, column, _] = animation_frame_tile.get_spritesheet_coordinate();
            animation_spritesheet_coordinates.push_back(SpritesheetCoordinate{
                .row = row,
                .column = column,
                .frame_duration_ms = (u32) animation_frame.duration,
            });
        }

        // Set the animation spritesheet coordinates on the tile to change it from static to animated
        tile.set_spritesheet_coordinates(animation_spritesheet_coordinates);
    }

    void TiledScene::initialize_tile_collision(const TiledTile& tiled_tile, u32 first_global_tile_id_in_tileset) {

    }

    void TiledScene::render_tile_layer(Renderer& renderer, const TiledLayer& layer) const {
        u32 tile_width = tiled_map->tilewidth;
        u32 tile_height = tiled_map->tileheight;

        u32 rows = layer.height;
        u32 columns = layer.width;

        for (u32 row = 0; row < rows; ++row) {
            for (u32 column = 0; column < columns; ++column) {
                u32 tile_index = row * columns + column;
                u32 global_tile_id = layer.data[tile_index];
                if (global_tile_id == 0) {
                    continue;
                }
                auto it = tiles.find(global_tile_id);
                if (it == tiles.end()) {
                    continue;
                }
                const Sprite& tile = it->second;

                f32 tile_x = (f32) column * (f32) tile_width;
                f32 tile_y = (f32) row * (f32) tile_height;

                Sprite::RenderConfig tile_render_config;
                tile_render_config.position = {tile_x, tile_y};
                tile_render_config.scale = SCALE;
                tile_render_config.debug = DEBUG;

                tile.render(renderer, tile_render_config);
            }
        }
    }

    void TiledScene::render_object_layer(Renderer& renderer, const TiledLayer& layer) const {

    }
}
