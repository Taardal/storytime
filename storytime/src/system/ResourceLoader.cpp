#include "Log.h"
#include "ResourceLoader.h"
#include <stb_image.h>

namespace storytime
{
    ResourceLoader::ResourceLoader(FileSystem* fileSystem)
            : fileSystem(fileSystem)
    {
    }

    Ref<Shader> ResourceLoader::LoadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) const
    {
        const std::string& vertexShaderSource = fileSystem->ReadFile(vertexShaderPath.c_str());
        const std::string& fragmentShaderSource = fileSystem->ReadFile(fragmentShaderPath.c_str());
        return CreateRef<Shader>(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
    }

    Ref<Texture> ResourceLoader::LoadTexture(const std::string& path) const
    {
        stbi_set_flip_vertically_on_load(0);
        Image image = LoadImage(path);
        Ref<Texture> texture = CreateRef<Texture>(image);
        FreeImage(image);
        return texture;
    }

    Image ResourceLoader::LoadImage(const std::string& path) const
    {
        int32_t width = 0;
        int32_t height = 0;
        int32_t channels = 0;
        int32_t desiredChannels = STBI_default;
        unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &channels, desiredChannels);
        return { pixels, width, height, channels };
    }

    void ResourceLoader::FreeImage(const Image& image) const
    {
        if (image.Pixels)
        {
            stbi_image_free(image.Pixels);
        }
    }

}
