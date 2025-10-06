#pragma once

#include "st_vulkan_buffer.h"
#include "st_vulkan_command_buffer.h"
#include "st_vulkan_device.h"

#include <stb_image.h>

namespace Storytime {
    struct VulkanTextureConfig {
        VulkanDevice* device = nullptr;
        std::string name = "VulkanTexture";
        u32 width = 0;
        u32 height = 0;

        void assert_valid() const {
            ST_ASSERT(device != nullptr, "device cannot be null");
            ST_ASSERT(width > 0, "width must be larger than zero");
            ST_ASSERT(height > 0, "height must be larger than zero");
        }
    };

    class VulkanTexture {
    public:
        typedef VulkanTextureConfig Config;

    private:
        Config config{};
        VkImage image = nullptr;
        VkImageView image_view = nullptr;
        VkDeviceMemory memory = nullptr;
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;

    public:
        VulkanTexture() = default;

        VulkanTexture(const Config& config);

        ~VulkanTexture();

        VulkanTexture(VulkanTexture&& other) noexcept;

        VulkanTexture(const VulkanTexture& other) = delete;

        VulkanTexture& operator=(VulkanTexture&& other) noexcept;

        VulkanTexture& operator=(const VulkanTexture& other) = delete;

        operator VkImage() const;

        VkImageView get_view() const;

        void set_pixels(const void* pixel_data, u64 pixel_data_size, const OnRecordCommandsFn& on_record_commands);

        void set_layout(VkImageLayout image_layout, const VulkanCommandBuffer& command_buffer);

    private:
        void copy_to_image(const VulkanBuffer& buffer, const VulkanCommandBuffer& command_buffer) const;

        void create_image();

        void destroy_image() const;

        void create_image_view();

        void destroy_image_view() const;

        void allocate_memory();

        void free_memory() const;
    };
}
