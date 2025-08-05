#include "st_resource_loader.h"

#include <stb_image.h>
#include <nlohmann/json.hpp>

namespace Storytime {
    ResourceLoader::ResourceLoader(ResourceLoaderConfig config) : config(std::move(config)) {
    }

    Shared<Shader> ResourceLoader::load_shader(const std::filesystem::path& vertex_shader_path, const std::filesystem::path& fragment_shader_path) const {
        ST_LOG_TRACE("Loading shader [{}, {}]", vertex_shader_path.c_str(), fragment_shader_path.c_str());

        const std::string& vertex_shader_source = config.file_reader->read(vertex_shader_path.c_str());
        ST_ASSERT(!vertex_shader_source.empty(), "Could not read vertex shader file [" << vertex_shader_path.c_str() << "]");

        const std::string& fragment_shader_source = config.file_reader->read(fragment_shader_path.c_str());
        ST_ASSERT(!fragment_shader_source.empty(), "Could not read fragment shader file [" << vertex_shader_path.c_str() << "]");

        auto shader = std::make_shared<Shader>(vertex_shader_source.c_str(), fragment_shader_source.c_str());
        ST_LOG_DEBUG("Loaded shader [{}, {}]", vertex_shader_path.c_str(), fragment_shader_path.c_str());
        return shader;
    }

    Shared<Texture> ResourceLoader::load_texture(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading texture [{}]", path.c_str());

        ST_ASSERT(!path.empty(), "Texture path must not be empty");
        ST_ASSERT(std::filesystem::exists(path), "Texture must exist on path [" << path << "]");

        Image image = load_image(path);
        auto texture = std::make_shared<Texture>(image);
        free_image(image);

        ST_LOG_DEBUG("Loaded texture [{}]", path.c_str());
        return texture;
    }

    Shared<Audio> ResourceLoader::load_audio(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading audio [{}]", path.c_str());

        ST_ASSERT(!path.empty(), "Audio path must not be empty");
        ST_ASSERT(std::filesystem::exists(path), "Audio must exist on path [" << path << "]");

        auto audio = std::make_shared<Audio>(config.audio_engine, path);

        ST_LOG_DEBUG("Loaded audio [{}]", path.c_str());
        return audio;
    }

    Shared<Spritesheet> ResourceLoader::load_spritesheet(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading spritesheet [{}]", path.c_str());

        ST_ASSERT(!path.empty(), "Spritesheet path must not be empty");
        ST_ASSERT(std::filesystem::exists(path), "Spritesheet must exist on path [" << path << "]");

        Shared<Texture> texture = load_texture(path);
        ST_ASSERT(texture != nullptr, "Spritesheet texture must be loaded");

        auto spritesheet = std::make_shared<Spritesheet>(SpritesheetConfig{
            .texture = texture,
        });

        ST_LOG_DEBUG("Loaded spritesheet [{}]", path.c_str());
        return spritesheet;
    }

    Shared<TiledProject> ResourceLoader::load_tiled_project(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading Tiled project [{}]", path.c_str());

        ST_ASSERT(!path.empty(), "Tiled project path must not be empty");
        ST_ASSERT(std::filesystem::exists(path), "Tiled project must exist on path [" << path << "]");

        std::string json = config.file_reader->read(path.c_str());
        ST_ASSERT(!json.empty(), "Could not read JSON for Tiled project [" << path.c_str() << "]");

        ST_TRY_THROW({
            return std::make_shared<TiledProject>(TiledProject::create(json));
        }, "Could not load Tiled project [" << path << "]");
    }

    Shared<TiledMap> ResourceLoader::load_tiled_map(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading Tiled map [{}]", path.c_str());

        ST_ASSERT(!path.empty(), "Tiled map path must not be empty");
        ST_ASSERT(std::filesystem::exists(path), "Tiled map must exist on path [" << path << "]");

        std::string json = config.file_reader->read(path.c_str());
        ST_ASSERT(!json.empty(), "Could not read JSON for Tiled map [" << path.c_str() << "]");

        ST_TRY_THROW({
            return std::make_shared<TiledMap>(TiledMap::create(json));
        }, "Could not load Tiled map [" << path << "]");
    }

    Shared<TiledTileset> ResourceLoader::load_tiled_tileset(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading Tiled tileset [{}]", path.c_str());
        ST_ASSERT(!path.empty(), "Tiled tileset path must not be empty");

        std::string json = config.file_reader->read(path.c_str());
        ST_ASSERT(!json.empty(), "Could not read JSON for Tiled tileset [" << path.c_str() << "]");

        ST_TRY_THROW({
            return std::make_shared<TiledTileset>(TiledTileset::create(json));
        }, "Could not load Tiled tileset [" << path << "]");
    }

    Shared<TiledObjectTemplate> ResourceLoader::load_tiled_object_template(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading Tiled template [{}]", path.c_str());
        ST_ASSERT(!path.empty(), "Tiled template path must not be empty");

        std::string json = config.file_reader->read(path.c_str());
        ST_ASSERT(!json.empty(), "Could not read JSON for Tiled template [" << path.c_str() << "]");

        ST_TRY_THROW({
            return std::make_shared<TiledObjectTemplate>(TiledObjectTemplate::create(json));
        }, "Could not load Tiled template [" << path << "]");
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
