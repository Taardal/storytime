#pragma once

#include "file_system.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"

namespace Storytime {
    class ResourceLoader {
    private:
        FileSystem* file_system;

    public:
        explicit ResourceLoader(FileSystem* fileSystem);

        Shared<Shader> load_shader(const std::string& vertex_shader_filename, const std::string& fragment_shader_filename) const;

        Shared<Texture> load_texture(const std::string& name) const;

    private:
        Image load_image(const std::string& path) const;

        void free_image(const Image& image) const;
    };
}
