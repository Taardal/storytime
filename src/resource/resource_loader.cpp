#include "resource_loader.h"

#include <stb_image.h>
#include <nlohmann/json.hpp>

namespace Storytime {
    ResourceLoader::ResourceLoader(ResourceLoaderConfig config) : config(std::move(config)) {
    }

    Shared<Shader> ResourceLoader::load_shader(const char* vertex_shader_path, const char* fragment_shader_path) const {
        const std::string& vertex_shader_source = config.file_system->read_file(vertex_shader_path);
        const std::string& fragment_shader_source = config.file_system->read_file(fragment_shader_path);
        return make_shared<Shader>(vertex_shader_source.c_str(), fragment_shader_source.c_str());
    }

    Shared<Texture> ResourceLoader::load_texture(const char* path) const {
        Image image = load_image(path);
        Shared<Texture> texture = make_shared<Texture>(image);
        free_image(image);
        return texture;
    }

    Shared<Audio> ResourceLoader::load_audio(const char* path) const {
        return make_shared<Audio>(config.audio_engine, path);
    }

    Shared<Spritesheet> ResourceLoader::load_spritesheet(const char* path, const SpritesheetConfig& config) const {
        SpritesheetConfig spritesheet_config = {
            .texture = load_texture(path),
            .sprite_width = config.sprite_width,
            .sprite_height = config.sprite_height,
        };
        return make_shared<Spritesheet>(spritesheet_config);
    }

    Shared<TiledMap> ResourceLoader::load_tiled_map(const char* path) const {
        std::string json = config.file_system->read_file(path);
        ST_ASSERT_THROW(!json.empty());

        TiledMap tiled_map;
        ST_EXECUTE_THROW(tiled_map = TiledMap::create(json));

        std::filesystem::path tiled_map_path(path);
        ST_ASSERT_THROW(!tiled_map_path.empty());

        std::string tiled_maps_directory_path = tiled_map_path.parent_path().string();
        ST_ASSERT_THROW(!tiled_maps_directory_path.empty());

        for (const TiledTilesetRef& tileset_ref : tiled_map.tileset_refs) {
            const std::string& tileset_file_relative_path = tileset_ref.source;
            ST_ASSERT_THROW(!tileset_file_relative_path.empty());

            std::stringstream ss;
            ss << tiled_maps_directory_path << "/" << tileset_file_relative_path;
            std::string tileset_path = ss.str();

            std::string tileset_json = config.file_system->read_file(tileset_path.c_str());
            ST_ASSERT_THROW(!tileset_json.empty());

            TiledTileset tileset = TiledTileset::create(tileset_json);
            tileset.firstgid = tileset_ref.firstgid;
            tiled_map.tilesets.push_back(tileset);
        }

        return make_shared<TiledMap>(tiled_map);
    }

    Image ResourceLoader::load_image(const char* path) const {
        int32_t width = 0;
        int32_t height = 0;
        int32_t channels = 0;
        int32_t desiredChannels = STBI_default;
        unsigned char* pixels = stbi_load(path, &width, &height, &channels, desiredChannels);
        return {pixels, width, height, channels};
    }

    void ResourceLoader::free_image(const Image& image) const {
        if (image.pixels) {
            stbi_image_free(image.pixels);
        }
    }
}
