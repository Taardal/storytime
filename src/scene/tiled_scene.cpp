#include "tiled_scene.h"
#include "scene/components.h"
#include "scene/direction.h"
#include "window/user_input.h"

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
            move_entity1(timestep, entity);
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
        u32 global_tile_id = first_global_tile_id_in_tileset + tiled_tile.id;
        tile_colliders.emplace(global_tile_id, tiled_tile.objectgroup.objects[0]);
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

                auto cit = tile_colliders.find(global_tile_id);
                if (cit == tile_colliders.end()) {
                    continue;
                }
                const TiledObject& tc = cit->second;

                Quad q;
                q.position = {tile_x + tc.x, tile_y + tc.y, 0.0f};
                q.size = { (f32) tc.width, (f32) tc.height };
                q.color = {1.0f, 0.0f, 1.0f, 1.0f};
                renderer.render_quad(q);
            }
        }
    }

    void TiledScene::render_object_layer(Renderer& renderer, const TiledLayer& layer) const {

    }

    void TiledScene::move_entity1(f64 timestep, entt::entity entity) {
        auto& entity_transform_component = entity_registry.get<TransformComponent>(entity);

        f32 entity_x = entity_transform_component.position.x;
        f32 entity_y = entity_transform_component.position.y;

        f32 entity_width = entity_transform_component.size.x;
        f32 entity_height = entity_transform_component.size.y;

        f32 entity_left = entity_x;
        f32 entity_right = entity_x + entity_width;
        f32 entity_top = entity_y;
        f32 entity_bottom = entity_y + entity_height;

        u32 entity_top_row = entity_top / tiled_map->tileheight;
        u32 entity_bottom_row = (entity_bottom - 1) / tiled_map->tileheight;
        u32 entity_left_column = entity_left / tiled_map->tilewidth;
        u32 entity_right_column = (entity_right - 1) / tiled_map->tilewidth;

        Direction direction_x = entity_transform_component.direction_x;
        Direction direction_y = entity_transform_component.direction_y;

        f32 move_speed = entity_transform_component.move_speed;

        f32 velocity_x = entity_transform_component.velocity_x;
        f32 velocity_y = entity_transform_component.velocity_y;

        f32 distance_x = velocity_x * (move_speed * timestep);
        f32 distance_y = velocity_y * (move_speed * timestep);

        f32 distance_to_closest_tile_x = std::numeric_limits<f32>::max();
        f32 distance_to_closest_tile_y = std::numeric_limits<f32>::max();

        u32 tile_width = tiled_map->tilewidth;
        u32 tile_height = tiled_map->tileheight;

        f32 tiled_map_right = tiled_map->width * tiled_map->tilewidth;
        f32 tiled_map_bottom = tiled_map->height * tiled_map->tileheight;

        for (const TiledLayer& layer : tiled_map->layers) {
            if (layer.type != "tilelayer") {
                continue;
            }
            u32 rows = layer.height;
            u32 columns = layer.width;

            if (direction_x == Direction::Right) {
                f32 distance_to_closest_tile = tiled_map_right - entity_right;
                for (i32 row = entity_top_row; row <= entity_bottom_row; ++row) {
                    for (i32 column = entity_right_column; column < columns; ++column) {
                        u32 tile_index = row * columns + column;
                        u32 global_tile_id = layer.data[tile_index];
                        if (global_tile_id == 0) {
                            continue;
                        }
                        if (auto it = tile_colliders.find(global_tile_id); it == tile_colliders.end()) {
                            continue;
                        }
                        f32 tile_left_x = column * tile_width;
                        f32 distance_to_tile = tile_left_x - entity_right;
                        if (distance_to_tile < distance_to_closest_tile) {
                            distance_to_closest_tile = distance_to_tile;
                            break;
                        }
                    }
                }
                distance_to_closest_tile_x = std::max(distance_to_closest_tile, 0.0f);
            }

            if (direction_x == Direction::Left) {
                f32 distance_to_closest_tile = -entity_x;
                for (i32 row = entity_top_row; row <= entity_bottom_row; ++row) {
                    for (i32 column = entity_left_column; column >= 0; --column) {
                        u32 tile_index = row * columns + column;
                        u32 global_tile_id = layer.data[tile_index];
                        if (global_tile_id == 0) {
                            continue;
                        }
                        if (auto it = tile_colliders.find(global_tile_id); it == tile_colliders.end()) {
                            continue;
                        }
                        f32 tile_right_x = column * tile_width + tile_width;
                        f32 distance_to_tile = tile_right_x - entity_left;
                        if (std::abs(distance_to_tile) < std::abs(distance_to_closest_tile)) {
                            distance_to_closest_tile = distance_to_tile;
                            break;
                        }
                    }
                }
                distance_to_closest_tile_x = std::min(distance_to_closest_tile, 0.0f);
            }

            if (direction_y == Direction::Up) {
                f32 distance_to_closest_tile = -entity_y;
                for (i32 column = entity_left_column; column <= entity_right_column; ++column) {
                    for (i32 row = entity_top_row; row >= 0; --row) {
                        u32 tile_index = row * columns + column;
                        u32 global_tile_id = layer.data[tile_index];
                        if (global_tile_id == 0) {
                            continue;
                        }
                        if (auto it = tile_colliders.find(global_tile_id); it == tile_colliders.end()) {
                            continue;
                        }
                        f32 tile_bottom = row * tile_height + tile_height;
                        f32 distance_to_tile = tile_bottom - entity_top;
                        if (std::abs(distance_to_tile) < std::abs(distance_to_closest_tile)) {
                            distance_to_closest_tile = distance_to_tile;
                            break;
                        }
                    }
                }
                distance_to_closest_tile_y = std::min(distance_to_closest_tile, 0.0f);
            }

            if (direction_y == Direction::Down) {
                f32 distance_to_closest_tile = tiled_map_bottom - entity_bottom;
                for (i32 column = entity_left_column; column <= entity_right_column; ++column) {
                    for (i32 row = entity_bottom_row; row < rows; ++row) {
                        u32 tile_index = row * columns + column;
                        u32 global_tile_id = layer.data[tile_index];
                        if (global_tile_id == 0) {
                            continue;
                        }
                        if (auto it = tile_colliders.find(global_tile_id); it == tile_colliders.end()) {
                            continue;
                        }
                        f32 tile_top = row * tile_height;
                        f32 distance_to_tile = tile_top - entity_bottom;
                        if (distance_to_tile < distance_to_closest_tile) {
                            distance_to_closest_tile = distance_to_tile;
                            break;
                        }
                    }
                }
                distance_to_closest_tile_y = std::max(distance_to_closest_tile, 0.0f);
            }
        }

        if (std::abs(distance_to_closest_tile_x) < std::abs(distance_x)) {
            distance_x = distance_to_closest_tile_x;
        }
        if (std::abs(distance_to_closest_tile_y) < std::abs(distance_y)) {
            distance_y = distance_to_closest_tile_y;
        }

        entity_transform_component.collision = distance_to_closest_tile_x == 0.0f || distance_to_closest_tile_y == 0.0f;

        entity_transform_component.position.x = entity_x + distance_x;
        entity_transform_component.position.y = entity_y + distance_y;
    }
}
