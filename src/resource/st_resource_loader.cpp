#include "st_resource_loader.h"

#include <stb_image.h>
#include <nlohmann/json.hpp>

namespace Storytime {
    ResourceLoader::ResourceLoader(ResourceLoaderConfig config) : config(std::move(config)) {
    }

    Shared<Texture> ResourceLoader::load_texture(const std::filesystem::path& path) const {
        ST_ASSERT(!path.empty(), "Texture path must not be empty");
        ST_ASSERT(std::filesystem::exists(path), "Texture must exist on path [" << path << "]");

        ST_LOG_TRACE("Loading texture [{}]", path.c_str());

        ImageFile image = load_image(path);
        auto texture = std::make_shared<Texture>(TextureConfig{ .image_file = image });
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

        std::string json = config.file_reader->read_string(path.c_str());
        ST_ASSERT(!json.empty(), "Could not read JSON for Tiled project [" << path.c_str() << "]");

        ST_TRY_THROW({
            return std::make_shared<TiledProject>(TiledProject::create(json));
        }, "Could not load Tiled project [" << path << "]");
    }

    Shared<TiledMap> ResourceLoader::load_tiled_map(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading Tiled map [{}]", path.c_str());

        ST_ASSERT(!path.empty(), "Tiled map path must not be empty");
        ST_ASSERT(std::filesystem::exists(path), "Tiled map must exist on path [" << path << "]");

        std::string json = config.file_reader->read_string(path.c_str());
        ST_ASSERT(!json.empty(), "Could not read JSON for Tiled map [" << path.c_str() << "]");

        ST_TRY_THROW({
            return std::make_shared<TiledMap>(TiledMap::create(json));
        }, "Could not load Tiled map [" << path << "]");
    }

    Shared<TiledTileset> ResourceLoader::load_tiled_tileset(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading Tiled tileset [{}]", path.c_str());
        ST_ASSERT(!path.empty(), "Tiled tileset path must not be empty");

        std::string json = config.file_reader->read_string(path.c_str());
        ST_ASSERT(!json.empty(), "Could not read JSON for Tiled tileset [" << path.c_str() << "]");

        ST_TRY_THROW({
            return std::make_shared<TiledTileset>(TiledTileset::create(json));
        }, "Could not load Tiled tileset [" << path << "]");
    }

    Shared<TiledObjectTemplate> ResourceLoader::load_tiled_object_template(const std::filesystem::path& path) const {
        ST_LOG_TRACE("Loading Tiled template [{}]", path.c_str());
        ST_ASSERT(!path.empty(), "Tiled template path must not be empty");

        std::string json = config.file_reader->read_string(path.c_str());
        ST_ASSERT(!json.empty(), "Could not read JSON for Tiled template [" << path.c_str() << "]");

        ST_TRY_THROW({
            return std::make_shared<TiledObjectTemplate>(TiledObjectTemplate::create(json));
        }, "Could not load Tiled template [" << path << "]");
    }

    ImageFile ResourceLoader::load_image(const std::filesystem::path& path) const {
        ImageFile image_file{};
        image_file.width = 0;
        image_file.height = 0;
        image_file.channels = 0;
        image_file.pixels = stbi_load(path.c_str(), &image_file.width, &image_file.height, &image_file.channels, STBI_rgb_alpha);
        return image_file;
    }

    void ResourceLoader::free_image(const ImageFile& image) const {
        if (image.pixels) {
            stbi_image_free(image.pixels);
        }
    }
}
