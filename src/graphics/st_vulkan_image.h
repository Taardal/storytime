#pragma once

#include "st_vulkan_buffer.h"
#include "st_vulkan_command_buffer.h"
#include "st_vulkan_device.h"

namespace Storytime {
    struct VulkanImageConfig {
        VulkanDevice* device = nullptr;
        std::string name = "VulkanImage";
        u32 width = 0;
        u32 height = 0;
        VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
        VkImageUsageFlags usage = 0;
        u32 mip_levels = 1;
    };

    class VulkanImage {
    public:
        typedef VulkanImageConfig Config;

    private:
        Config config{};
        VkImage image = nullptr;
        VkImageView image_view = nullptr;
        VkDeviceMemory memory = nullptr;
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;

    public:
        VulkanImage() = default;

        VulkanImage(const Config& config);

        ~VulkanImage();

        VulkanImage(VulkanImage&& other) noexcept;

        VulkanImage(const VulkanImage& other) = delete;

        VulkanImage& operator=(VulkanImage&& other) noexcept;

        VulkanImage& operator=(const VulkanImage& other) = delete;

        operator VkImage() const;

        VkImageView get_view() const;

        VkFormat get_format() const;

        void set_data(const void* data, u64 data_size, const OnRecordCommandsFn& on_record_commands);

        void set_layout(VkImageLayout image_layout, const VulkanCommandBuffer& command_buffer);

    private:
        void copy_to_image(const VulkanBuffer& buffer, const VulkanCommandBuffer& command_buffer) const;

        void generate_mipmaps(const VulkanCommandBuffer& command_buffer) const;

        struct TransitionLayoutConfig {
            VulkanCommandBuffer command_buffer = nullptr;
            VkImageLayout src_layout = VK_IMAGE_LAYOUT_UNDEFINED;
            VkImageLayout dst_layout = VK_IMAGE_LAYOUT_UNDEFINED;
            VkAccessFlags src_access = 0;
            VkAccessFlags dst_access = 0;
            VkPipelineStageFlags src_stage = 0;
            VkPipelineStageFlags dst_stage = 0;
            u32 mip_level = 0;
        };

        void transition_layout(const TransitionLayoutConfig& config) const;

        void create_image();

        void destroy_image() const;

        void create_image_view();

        void destroy_image_view() const;

        void allocate_memory();

        void free_memory() const;
    };
}
