#pragma once

#include "Core.h"
#include "FileSystem.h"
#include "graphics/Shader.h"

namespace storytime
{
    class ResourceLoader
    {
    private:
        FileSystem* fileSystem;
    public:
        explicit ResourceLoader(FileSystem* fileSystem);

        virtual ~ResourceLoader();

        Ref<Shader> LoadShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
    };
}


