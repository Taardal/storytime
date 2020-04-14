#pragma once

#include "Core.h"
#include "FileSystem.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"

namespace storytime
{
    class ResourceLoader
    {
    private:
        FileSystem* fileSystem;
    public:
        explicit ResourceLoader(FileSystem* fileSystem);

        virtual ~ResourceLoader();

        [[nodiscard]] Ref<Shader> LoadShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) const;

        [[nodiscard]] Ref<Texture> LoadTexture(const std::string& name) const;

    private:
        [[nodiscard]] Image LoadImage(const std::string& path) const;

        void FreeImage(const Image& image) const;
    };
}


