#pragma once

#include "st_vulkan_buffer.h"
#include "st_vulkan_command_buffer.h"
#include "st_vulkan_device.h"

namespace Storytime {
    struct VulkanTextureConfig {
        VulkanDevice* device = nullptr;
        VkCommandBuffer command_buffer = nullptr;
        std::string name = "VulkanTexture";
        std::filesystem::path path;
    };

    class VulkanTexture {
    public:
        typedef VulkanTextureConfig Config;

    private:
        Config config{};
        VulkanBuffer staging_buffer{};
        VkImage image = nullptr;
        VkImageView image_view = nullptr;
        VkDeviceMemory memory = nullptr;
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
        i32 width = 0;
        i32 height = 0;
        i32 channels = 0;

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

        void set_layout(VkImageLayout image_layout, const VulkanCommandBuffer& command_buffer);

        void copy_to_image(const VulkanBuffer& buffer, const VulkanCommandBuffer& command_buffer) const;

    private:
        void create_image();

        void destroy_image() const;

        void create_image_view();

        void destroy_image_view() const;

        void allocate_memory();

        void free_memory() const;
    };
}
