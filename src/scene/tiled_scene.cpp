#include "tiled_scene.h"

namespace Storytime {
    TiledScene::TiledScene(const TiledSceneConfig& config) : config(config) {
        ST_ASSERT(config.file_system != nullptr, "Invalid file_system");
        ST_ASSERT(config.resource_loader != nullptr, "Invalid resource_loader");
        ST_ASSERT(config.renderer != nullptr, "Invalid renderer");
        ST_ASSERT(!config.map_path.empty(), "Invalid tiled_map_path");
    }

    void TiledScene::on_initialize() {
        tiled_map = config.resource_loader->load_tiled_map(config.map_path.c_str());
        ST_ASSERT(tiled_map != nullptr, "Tiled map was not loaded");
        initialize_tiles();
    }

    void TiledScene::on_update(f64 timestep) {
        update_tiles(timestep);
    }

    void TiledScene::on_render() {
        for (const TiledLayer& layer : tiled_map->layers) {
            if (!layer.visible) {
                continue;
            }
            if (layer.type != "tilelayer") {
                continue;
            }
            render_tile_layer(layer);
        }
    }

    void TiledScene::initialize_tiles() {
        // Tileset paths are relative to the map that is using them
        auto tiled_map_directory_path = config.map_path.parent_path();

        // Tileset refs
        for (const TiledTilesetRef& tileset_ref : tiled_map->tilesetrefs) {
            // Get the path to the tileset json file relative to the map
            auto tileset_path = std::filesystem::path(tiled_map_directory_path).append(tileset_ref.source);

            std::string tileset_json = config.file_system->read_file(tileset_path.c_str());
            if (tileset_json.empty()) {
                ST_THROW("Could not read JSON for tileset [" << tileset_path.c_str() << "]");
            }

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
        ST_ASSERT(tile_iterator != tiles.end(), "Cannot initialize animation for tile that does not exist");
        Sprite& tile = tile_iterator->second;

        // Get spritesheet coordinates for all sprites in animation
        std::vector<SpritesheetCoordinate> animation_spritesheet_coordinates;
        for (auto& animation_frame : tiled_tile.animation) {

            // Find static sprite for the current frame of the animation
            u32 animation_frame_tile_global_tile_id = first_global_tile_id_in_tileset + animation_frame.tileid;
            auto animation_frame_tile_iterator = tiles.find(animation_frame_tile_global_tile_id);
            ST_ASSERT(animation_frame_tile_iterator != tiles.end(), "Cannot use tile that does not exist for animation sequence");
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

    void TiledScene::update_tiles(f64 timestep) {
        for (auto& [global_tile_id, tile] : tiles) {
            if (!tile.is_animated()) {
                continue;
            }
            tile.update(timestep);
        }
    }

    void TiledScene::render_tile_layer(const TiledLayer& layer) const {
        f32 tile_width = (f32) tiled_map->tilewidth;
        f32 tile_height = (f32) tiled_map->tileheight;

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

                f32 tile_x = (f32) column * tile_width;
                f32 tile_y = (f32) row * tile_height;

                //
                // Problem:
                // The renderer will offset each quad left and up by half its size to put its origin in the center,
                // but this will make each quad appear slightly off-position compared to Tiled Editor.
                //
                // Solution:
                // Offset the position of each sprite right and down by half its size so that the quad offsets done by
                // the renderer will make them appear at the correct position
                //
                tile_x += tile_width / 2;
                tile_y += tile_height / 2;

                Sprite::RenderConfig tile_render_config;
                tile_render_config.position = {tile_x, tile_y};
                tile_render_config.scale = config.scale;
                tile_render_config.debug = config.debug;

                ((Sprite&) tile).render(config.renderer, tile_render_config);

                auto cit = tile_colliders.find(global_tile_id);
                if (cit == tile_colliders.end()) {
                    continue;
                }
                const TiledObject& tc = cit->second;

                f32 tile_collider_width = (f32) tc.width;
                f32 tile_collider_height = (f32) tc.height;

                f32 tile_collider_x = tile_x + tc.x;
                f32 tile_collider_y = tile_y + tc.y;

                Quad q;
                q.position = {tile_collider_x, tile_collider_y, 0.0f};
                q.size = { tile_collider_width, tile_collider_height };
                q.color = {1.0f, 0.0f, 1.0f, 1.0f};
                config.renderer->render_quad(q);
            }
        }
    }
}
