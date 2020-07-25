#pragma once

#include <cstdint>

namespace storytime
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
        uint32_t colorAttachmentId;
        uint32_t depthAttachmentId;

    public:
        Framebuffer(const Config& config);

        virtual ~Framebuffer();

        const Config& GetConfig() const;

        uint32_t GetColorAttachmentId() const;

        void Bind();

        void Unbind();

        void Resize(uint32_t width, uint32_t height);

    private:
        void Invalidate();
    };
}


