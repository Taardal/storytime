#pragma once

#include <string>

namespace storytime
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
    private:
        static const int32_t TARGET;
        static const int32_t LEVEL_OF_DETAIL;
        static const int32_t BORDER;

        uint32_t id;
        uint32_t width;
        uint32_t height;
        int32_t format;
        int32_t internalFormat;

    public:
        explicit Texture(const Image& image);

        Texture(uint32_t width, uint32_t height);

        ~Texture();

        void SetPixels(const void* data) const;

        void Bind(uint32_t textureSlot = 0) const;

        void Unbind() const;

    private:
        void CreateTexture();

        static int32_t GetFormat(int32_t channels = 4);

        static int32_t GetInternalFormat(int32_t channels = 4);
    };
}


