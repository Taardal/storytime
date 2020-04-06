#include "Log.h"
#include "ResourceLoader.h"

namespace storytime
{
    ResourceLoader::ResourceLoader(FileSystem* fileSystem)
            : fileSystem(fileSystem)
    {
        ST_TRACE(ST_TAG, "Created");
    }

    ResourceLoader::~ResourceLoader()
    {
        ST_TRACE(ST_TAG, "Destroyed");
    }

    Ref<Shader> ResourceLoader::LoadShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
    {
        static const std::string& path = "assets/shaders/";
        const std::string& vertexShaderSource = fileSystem->ReadFile((path + vertexShaderFilename).c_str());
        const std::string& fragmentShaderSource = fileSystem->ReadFile((path + fragmentShaderFilename).c_str());
        return CreateRef<Shader>(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
    }

}
