#pragma once

#include <string>

namespace Storytime
{
    struct Image
    {
        unsigned char* Pixels;
        int32_t Width;
        int32_t Height;
        int32_t Channels;
    };

    class Texture
    {
    public:
        struct Config
        {
            Config();

            uint32_t Width;
            uint32_t Height;
            int32_t Format;
            int32_t InternalFormat;
            int32_t LevelOfDetail;
            int32_t Border;
        };

    private:
        static const int32_t TARGET;

        uint32_t id;
        uint32_t width;
        uint32_t height;
        int32_t format;
        int32_t internalFormat;
        int32_t levelOfDetail;
        int32_t border;

    public:
        Texture(const Config& config);

        Texture(const Image& image);

        Texture(uint32_t width, uint32_t height);

        ~Texture();

        uint32_t GetId() const;

        int32_t GetLevelOfDetail() const;

        void SetPixels(const void* data) const;

        void Bind(uint32_t textureSlot = 0) const;

        void Unbind() const;

    private:
        void Init();
    };
}


