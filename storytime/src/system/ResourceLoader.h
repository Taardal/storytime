#pragma once

#include "Core.h"
#include "FileSystem.h"
#include "CoordinateSystem.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"

namespace storytime
{
    class ResourceLoader
    {
    private:
        FileSystem* fileSystem;
        CoordinateSystem coordinateSystem;

    public:
        ResourceLoader(FileSystem* fileSystem, CoordinateSystem coordinateSystem);

        Ref<Shader> LoadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) const;

        Ref<Texture> LoadTexture(const std::string& path) const;

    private:
        Image LoadImage(const std::string& path) const;

        void FreeImage(const Image& image) const;
    };
}