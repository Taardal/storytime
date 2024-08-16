#pragma once

namespace Storytime {
    struct Image {
        unsigned char* pixels;
        i32 width;
        i32 height;
        i32 channels;
    };

    struct TextureConfig {
        u32 width = 0;
        u32 height = 0;
        i32 format = 0;
        i32 internal_format = 0;
        i32 level_of_detail = 0;
        i32 border = 0;
    };

    class Texture {
    private:
        static const i32 TARGET;

    private:
        u32 id;
        u32 width;
        u32 height;
        i32 format;
        i32 internal_format;
        i32 level_of_detail;
        i32 border;

    public:
        explicit Texture(const TextureConfig& config);

        explicit Texture(const Image& image);

        Texture(u32 width, u32 height);

        ~Texture();

        u32 get_id() const;

        i32 get_level_of_detail() const;

        void set_pixels(const void* data) const;

        void bind(u32 texture_slot = 0) const;

        void unbind() const;

    private:
        void initialize();
    };
}
