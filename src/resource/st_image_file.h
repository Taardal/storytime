#pragma once

namespace Storytime {
    struct ImageFile {
        i32 width = 0;
        i32 height = 0;
        i32 channels = 0;
        void* pixels = nullptr;

        // ImageFile() = default;
        //
        // ~ImageFile();
        //
        // ImageFile(const ImageFile& other) = delete;
        //
        // ImageFile(ImageFile&& other) noexcept;
        //
        // ImageFile& operator=(const ImageFile& other) = delete;
        //
        // ImageFile& operator=(ImageFile&& other) noexcept;
    
        u64 get_byte_size() const;
    };
}
