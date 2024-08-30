#pragma once

#include "system/file_system.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "audio/audio_engine.h"
#include "audio/audio.h"

namespace Storytime {
    struct ResourceLoaderConfig {
        FileSystem* file_system = nullptr;
        AudioEngine* audio_engine = nullptr;
    };

    class ResourceLoader {
    private:
        ResourceLoaderConfig config;

    public:
        explicit ResourceLoader(ResourceLoaderConfig config);

        Shared<Shader> load_shader(const char* vertex_shader_path, const char* fragment_shader_path) const;

        Shared<Texture> load_texture(const char* path) const;

        Shared<Audio> load_audio(const char* path) const;

    private:
        Image load_image(const char* path) const;

        void free_image(const Image& image) const;
    };
}