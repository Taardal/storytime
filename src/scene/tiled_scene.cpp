#include "tiled_scene.h"

namespace Storytime {
    constexpr f32 TiledScene::SCALE = 2.0f;
    constexpr bool TiledScene::DEBUG = false;

    TiledScene::TiledScene(const TiledSceneConfig& config)
        : config(config), tiled_map(config.resource_loader->load_tiled_map(config.tiled_map_path.string().c_str())) {
        initialize_tiles();
    }

    void TiledScene::update(f64 timestep) {
        for (auto& [global_tile_id, tile] : tiles) {
            if (!tile.is_animated()) {
                continue;
            }
            tile.update(timestep);
        }
    }

    void TiledScene::render(Renderer& renderer) const {
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
            initialize_tiles_from_tileset(tileset, tileset_image_path);
        }

        // Tilesets
        for (const TiledTileset& tileset : tiled_map->tilesets) {
            // Get the path to the tileset image file relative to the map
            auto tileset_image_path = std::filesystem::path(tiled_map_directory_path).append(tileset.image);
            initialize_tiles_from_tileset(tileset, tileset_image_path);
        }
    }

    void TiledScene::initialize_tiles_from_tileset(
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

        // Go through all animated tiles in tileset and update previously created static sprites to be animated
        // sprites. This must be done after all tiles has been created because we need to be able to look up all static
        // sprites needed for the animation sequence.
        for (const TiledTile& tiled_tile : tileset.tiles) {
            std::vector<TiledAnimationFrame> animation_frames = tiled_tile.animation;
            if (animation_frames.size() == 0) {
                continue;
            }

            // Find static sprite that will be updated to be animated
            u32 global_tile_id = first_global_tile_id_in_tileset + tiled_tile.id;
            auto tile_iterator = tiles.find(global_tile_id);
            ST_ASSERT_THROW(tile_iterator != tiles.end());
            Sprite& tile = tile_iterator->second;

            // Get spritesheet coordinates for all sprites in animation
            std::vector<SpritesheetCoordinate> animation_spritesheet_coordinates;
            for (auto& animation_frame : animation_frames) {

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
        for (const TiledObject& object : layer.objects) {
            if (!object.visible) {
                continue;
            }

            f32 x = object.x;
            f32 y = object.y;

            f32 scale = SCALE;

            f32 quad_x = x * scale;
            f32 quad_y = y * scale;

            u32 sprite_width = 16.0f; // (f32) object.width;
            u32 sprite_height = 16.0f; // (f32) object.height;

            f32 quad_width = sprite_width * scale;
            f32 quad_height = sprite_height * scale;

            Quad quad;
            quad.color = {0.0f, 1.0f, 0.0f, 1.0f};
            quad.position = {quad_x, quad_y, 0.0f};
            quad.size = {quad_width, quad_height};
            renderer.render_quad(quad);
        }
    }
}
