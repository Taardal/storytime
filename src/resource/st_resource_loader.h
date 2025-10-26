#pragma once

#include "audio/st_audio.h"
#include "audio/st_audio_engine.h"
#include "graphics/st_spritesheet.h"
#include "graphics/st_vulkan_command_pool.h"
#include "resource/st_image_file.h"
#include "system/st_file_reader.h"
#include "tiled/st_tiled_map.h"
#include "tiled/st_tiled_project.h"
#include "tiled/st_tiled_template.h"

#define ST_RES_PATH(relative_path) std::filesystem::path(ST_RES_DIR) / std::filesystem::path(relative_path)

namespace Storytime {
    struct ResourceLoaderConfig {
        FileReader* file_reader = nullptr;
        AudioEngine* audio_engine = nullptr;
        VulkanDevice* vulkan_device = nullptr;

        const ResourceLoaderConfig& assert_valid() const {
            ST_ASSERT_NOT_NULL(file_reader);
            ST_ASSERT_NOT_NULL(audio_engine);
            ST_ASSERT_NOT_NULL(vulkan_device);
            return *this;
        }
    };

    class ResourceLoader {
    private:
        ResourceLoaderConfig config;
        VulkanCommandPool vulkan_command_pool;

    public:
        ResourceLoader(const ResourceLoaderConfig& config);

        Shared<Texture> load_texture(const std::filesystem::path& path) const;

        Shared<Audio> load_audio(const std::filesystem::path& path) const;

        Shared<Spritesheet> load_spritesheet(const std::filesystem::path& path) const;

        Shared<TiledProject> load_tiled_project(const std::filesystem::path& path) const;

        Shared<TiledMap> load_tiled_map(const std::filesystem::path& path) const;

        Shared<TiledTileset> load_tiled_tileset(const std::filesystem::path& path) const;

        Shared<TiledObjectTemplate> load_tiled_object_template(const std::filesystem::path& path) const;

    private:
        ImageFile load_image(const std::filesystem::path& path) const;

        void free_image(const ImageFile& image) const;
    };
}
