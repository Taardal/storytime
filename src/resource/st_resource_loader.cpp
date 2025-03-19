#include "st_resource_loader.h"

#include <stb_image.h>
#include <nlohmann/json.hpp>

namespace Storytime {
    ResourceLoader::ResourceLoader(ResourceLoaderConfig config) : config(std::move(config)) {
    }

    Shared<Shader> ResourceLoader::load_shader(const std::filesystem::path& vertex_shader_path, const std::filesystem::path& fragment_shader_path) const {
        ST_LOG_TRACE("Loading shader [{}, {}]", vertex_shader_path.c_str(), fragment_shader_path.c_str());
        const std::string& vertex_shader_source = config.file_reader->read_file(vertex_shader_path.c_str());
        const std::string& fragment_shader_source = config.file_reader->read_file(fragment_shader_path.c_str());
        auto shader = shared<Shader>(vertex_shader_source.c_str(), fragment_shader_source.c_str());
        ST_LOG_DEBUG("Loaded shader [{}, {}]", vertex_shader_path.c_str(), fragment_shader_path.c_str());
        return shader;
    }

    Shared<Texture> ResourceLoader::load_texture(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading texture [{}]", path.c_str());
        Image image = load_image(path);
        Shared<Texture> texture = shared<Texture>(image);
        free_image(image);
        ST_LOG_DEBUG("Loaded texture [{}]", path.c_str());
        return texture;
    }

    Shared<Audio> ResourceLoader::load_audio(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading audio [{}]", path.c_str());
        auto audio = shared<Audio>(config.audio_engine, path);
        ST_LOG_DEBUG("Loaded audio [{}]", path.c_str());
        return audio;
    }

    Shared<Spritesheet> ResourceLoader::load_spritesheet(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading spritesheet [{}]", path.c_str());
        SpritesheetConfig spritesheet_config = {
            .texture = load_texture(path),
        };
        auto spritesheet = shared<Spritesheet>(spritesheet_config);
        ST_LOG_DEBUG("Loaded spritesheet [{}]", path.c_str());
        return spritesheet;
    }

    Shared<TiledProject> ResourceLoader::load_tiled_project(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading Tiled project [{}]", path.c_str());
        ST_ASSERT(!path.empty(), "Tiled project path must not be empty");

        std::string json = config.file_reader->read_file(path.c_str());
        ST_ASSERT_THROW(!json.empty(), "Could not read JSON for tiled project [" << path.c_str() << "]");

        ST_TRY_THROW({
            return shared<TiledProject>(TiledProject::create(json));
        }, "Could not load tiled project");
    }

    Shared<TiledMap> ResourceLoader::load_tiled_map(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading Tiled map [{}]", path.c_str());
        ST_ASSERT(!path.empty(), "Tiled map path must not be empty");

        std::string json = config.file_reader->read_file(path.c_str());
        ST_ASSERT_THROW(!json.empty(), "Could not read JSON for tiled map [" << path.c_str() << "]");

        ST_TRY_THROW({
            return shared<TiledMap>(TiledMap::create(json));
        }, "Could not load tiled map");
    }

    Shared<TiledTileset> ResourceLoader::load_tiled_tileset(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading Tiled tileset [{}]", path.c_str());
        ST_ASSERT(!path.empty(), "Tiled tileset path must not be empty");

        std::string json = config.file_reader->read_file(path.c_str());
        ST_ASSERT_THROW(!json.empty(), "Could not read JSON for tiled tileset [" << path.c_str() << "]");

        ST_TRY_THROW({
            return shared<TiledTileset>(TiledTileset::create(json));
        }, "Could not load tiled map");
    }

    Image ResourceLoader::load_image(const std::filesystem::path& path) const {
        int32_t width = 0;
        int32_t height = 0;
        int32_t channels = 0;
        int32_t desiredChannels = STBI_default;
        unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &channels, desiredChannels);
        return {pixels, width, height, channels};
    }

    void ResourceLoader::free_image(const Image& image) const {
        if (image.pixels) {
            stbi_image_free(image.pixels);
        }
    }
}
