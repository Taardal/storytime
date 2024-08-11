#pragma once

#include "Texture.h"
#include <cstdint>

namespace Storytime
{
    class Framebuffer
    {
    public:
        struct Config
        {
            uint32_t Width;
            uint32_t Height;
            uint32_t Samples;
            bool SwapChainTarget;

            Config();
        };

    private:
        Config config;
        uint32_t id;
        Texture* colorAttachmentTexture;
        Texture* depthAttachmentTexture;

    public:
        Framebuffer(const Config& config);

        virtual ~Framebuffer();

        const Config& GetConfig() const;

        Texture* GetColorAttachmentTexture() const;

        void Bind();

        void Unbind();

        void Resize(uint32_t width, uint32_t height);

    private:
        void Init();

        void AttachTexture(uint32_t attachment, Texture* texture) const;

        void Delete() const;
    };
}


