#pragma once

#include "graphics/st_vulkan_image.h"
#include "resource/st_image_file.h"

namespace Storytime {
    struct TextureConfig {
        ImageFile image_file{};
        std::shared_ptr<VulkanImage> vulkan_image = nullptr;
    };

    class Texture {
    public:
        typedef TextureConfig Config;

    private:
        Config config{};

    public:
        Texture(const Config& config);

        u32 get_width() const;

        u32 get_height() const;

        // std::pair<u32, u32> get_size() const;
        //
        // u32 get_id() const;
        //
        // i32 get_level_of_detail() const;
        //
        // void set_pixels(const void* data) const;
        //
        // void bind(u32 texture_slot = 0) const;
        //
        // void unbind() const;
    };
}
