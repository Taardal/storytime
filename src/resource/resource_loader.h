#pragma once

#include "audio/audio.h"
#include "audio/audio_engine.h"
#include "graphics/shader.h"
#include "graphics/spritesheet.h"
#include "graphics/texture.h"
#include "system/file_reader.h"
#include "tiled/tiled_map.h"
#include "tiled/tiled_project.h"

#include <memory>

namespace Storytime {
    struct ResourceLoaderConfig {
        FileReader* file_reader = nullptr;
        AudioEngine* audio_engine = nullptr;
    };

    class ResourceLoader {
    private:
        ResourceLoaderConfig config;

    public:
        explicit ResourceLoader(ResourceLoaderConfig config);

        Shared<Shader> load_shader(const std::filesystem::path& vertex_shader_path, const std::filesystem::path& fragment_shader_path) const;

        Shared<Texture> load_texture(const std::filesystem::path& path) const;

        Shared<Audio> load_audio(const std::filesystem::path& path) const;

        Shared<Spritesheet> load_spritesheet(const std::filesystem::path& path) const;

        Shared<TiledProject> load_tiled_project(const std::filesystem::path& path) const;

        Shared<TiledMap> load_tiled_map(const std::filesystem::path& path) const;

        Shared<TiledTileset> load_tiled_tileset(const std::filesystem::path& path) const;

    private:
        Image load_image(const std::filesystem::path& path) const;

        void free_image(const Image& image) const;
    };
}
