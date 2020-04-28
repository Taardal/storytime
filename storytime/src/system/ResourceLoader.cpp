#include "Log.h"
#include "ResourceLoader.h"
#include <stb_image.h>

namespace storytime
{
    ResourceLoader::ResourceLoader(FileSystem* fileSystem)
            : fileSystem(fileSystem)
    {
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    ResourceLoader::~ResourceLoader()
    {
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    Ref<Shader> ResourceLoader::LoadShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) const
    {
        static const std::string& path = "res/shaders/";
        const std::string& vertexShaderSource = fileSystem->ReadFile((path + vertexShaderFilename).c_str());
        const std::string& fragmentShaderSource = fileSystem->ReadFile((path + fragmentShaderFilename).c_str());
        return CreateRef<Shader>(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
    }

    Ref<Texture> ResourceLoader::LoadTexture(const std::string& name) const
    {
        static const std::string& path = "res/textures/";
        stbi_set_flip_vertically_on_load(1);
        Image image = LoadImage(path + name);
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
