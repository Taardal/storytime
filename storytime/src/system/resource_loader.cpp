#include "resource_loader.h"
#include <stb_image.h>

namespace Storytime {
    ResourceLoader::ResourceLoader(FileSystem* fileSystem) : file_system(fileSystem) {
    }

    Shared<Shader> ResourceLoader::load_shader(const std::string& vertex_shader_filename, const std::string& fragment_shader_filename) const {
        static const std::string& path = "res/shaders/";
        const std::string& vertex_shader_source = file_system->read_file((path + vertex_shader_filename).c_str());
        const std::string& fragment_shader_source = file_system->read_file((path + fragment_shader_filename).c_str());
        return make_shared<Shader>(vertex_shader_source.c_str(), fragment_shader_source.c_str());
    }

    Shared<Texture> ResourceLoader::load_texture(const std::string& name) const {
        static const std::string& path = "res/textures/";
        stbi_set_flip_vertically_on_load(1);
        Image image = load_image(path + name);
        Shared<Texture> texture = make_shared<Texture>(image);
        free_image(image);
        return texture;
    }

    Image ResourceLoader::load_image(const std::string& path) const {
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
