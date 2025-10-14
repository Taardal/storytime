#include "st_image_file.h"

#include <stb_image.h>

namespace Storytime {
    // ImageFile::~ImageFile() {
    //     if (pixels == nullptr) {
    //         return;
    //     }
    //     stbi_image_free(pixels);
    //     pixels = nullptr;
    // }
    //
    // ImageFile::ImageFile(ImageFile&& other) noexcept
    //         : width(other.width),
    //           height(other.height),
    //           channels(other.channels),
    //           pixels(other.pixels)
    // {
    //     other.pixels = nullptr;
    // }
    //
    // ImageFile& ImageFile::operator=(ImageFile&& other) noexcept {
    //     if (this != &other) {
    //         width = other.width;
    //         height = other.height;
    //         channels = other.channels;
    //         pixels = other.pixels;
    //         other.pixels = nullptr;
    //     }
    //     return *this;
    // }
}
