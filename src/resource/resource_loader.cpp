#include "resource_loader.h"

#include <stb_image.h>
#include <nlohmann/json.hpp>

namespace Storytime {
    ResourceLoader::ResourceLoader(ResourceLoaderConfig config) : config(std::move(config)) {
    }

    Shared<Shader> ResourceLoader::load_shader(const char* vertex_shader_path, const char* fragment_shader_path) const {
        ST_LOG_TRACE("Loading shader [{}, {}]", vertex_shader_path, fragment_shader_path);
        const std::string& vertex_shader_source = config.file_system->read_file(vertex_shader_path);
        const std::string& fragment_shader_source = config.file_system->read_file(fragment_shader_path);
        auto shader = shared<Shader>(vertex_shader_source.c_str(), fragment_shader_source.c_str());
        ST_LOG_DEBUG("Loaded shader [{}, {}]", vertex_shader_path, fragment_shader_path);
        return shader;
    }

    Shared<Texture> ResourceLoader::load_texture(const char* path) const {
        ST_LOG_TRACE("Loading texture [{}]", path);
        Image image = load_image(path);
        Shared<Texture> texture = shared<Texture>(image);
        free_image(image);
        ST_LOG_DEBUG("Loaded texture [{}]", path);
        return texture;
    }

    Shared<Audio> ResourceLoader::load_audio(const char* path) const {
        ST_LOG_TRACE("Loading audio [{}]", path);
        auto audio = shared<Audio>(config.audio_engine, path);
        ST_LOG_DEBUG("Loaded audio [{}]", path);
        return audio;
    }

    Shared<Spritesheet> ResourceLoader::load_spritesheet(const char* path) const {
        ST_LOG_TRACE("Loading spritesheet [{}]", path);
        SpritesheetConfig spritesheet_config = {
            .texture = load_texture(path),
        };
        auto spritesheet = shared<Spritesheet>(spritesheet_config);
        ST_LOG_DEBUG("Loaded spritesheet [{}]", path);
        return spritesheet;
    }

    Shared<TiledMap> ResourceLoader::load_tiled_map(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading Tiled map [{}]", path.c_str());
        ST_ASSERT(!path.empty(), "Tiled map path must not be empty");

        std::string json = config.file_system->read_file(path.c_str());
        if (json.empty()) {
            ST_THROW("Could not read JSON for tiled map [" << path.c_str() << "]");
        }

        ST_TRY_THROW({
            TiledMap tiled_map = TiledMap::create(json);
            return make_shared<TiledMap>(tiled_map);
        }, "Could not load tiled map");
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
