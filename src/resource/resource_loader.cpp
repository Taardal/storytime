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

    Shared<Spritesheet> ResourceLoader::load_spritesheet(const char* path) const {
        SpritesheetConfig spritesheet_config = {
            .texture = load_texture(path),
        };
        return make_shared<Spritesheet>(spritesheet_config);
    }

    Shared<TiledMap> ResourceLoader::load_tiled_map(const std::filesystem::path& path) const {
        std::string path_string = path.string();
        ST_LOG_TRACE("Loading tiled map [{}]", path_string);

        std::string json = config.file_system->read_file(path.c_str());
        ST_ASSERT_THROW(!json.empty());

        TiledMap tiled_map;
        ST_EXECUTE_THROW(tiled_map = TiledMap::create(json));

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
