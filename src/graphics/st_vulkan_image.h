#pragma once

#include "st_vulkan_command_buffer.h"
#include "st_vulkan_device.h"

namespace Storytime {
    struct VulkanImageConfig {
        VulkanDevice* device = nullptr;
        std::string name = "VulkanImage";
        u32 width = 0;
        u32 height = 0;
        u32 depth = 1;
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

        void set_pixels(const OnRecordCommandsFn& on_record_commands, u64 pixel_data_size, const void* pixel_data);

    private:
        struct LayoutTransition {
            VkImageLayout src_layout = VK_IMAGE_LAYOUT_UNDEFINED;
            VkImageLayout dst_layout = VK_IMAGE_LAYOUT_UNDEFINED;

            /// Memory access masks:
            /// - Specifies which types of memory operations must be synchronized.
            /// - Used to declare hazards that must be resolved before/after the transition.
            VkAccessFlags src_access = 0; /// Operations to wait on before the transition (previous accesses).
            VkAccessFlags dst_access = 0; /// Operations that will use the image after the transition (next accesses).

            /// Pipeline stage masks:
            /// - Specifies where in the GPU pipeline the synchronization occurs.
            /// - Define the scope of the access masks: i.e., *when* those reads/writes are relevant.
            VkPipelineStageFlags src_stage = 0; /// Earliest pipeline stage that could have performed `src_access` operations.
            VkPipelineStageFlags dst_stage = 0; /// Earliest pipeline stage that will perform `dst_access` operations.

            /// The first mip level the transition will be applied to.
            u32 first_mip_level = 0; /// Level 0 is the base image.

            /// The number of mip levels the transition will be applied to, starting from the first mip level.
            u32 mip_level_count = 0; /// Defaults to the image's mip level count.
        };

        void transition_layout(const VulkanCommandBuffer& command_buffer, const LayoutTransition& layout_transition) const;

        struct CopyToImage {
            VkBuffer buffer = nullptr;

            /// The layout that the image must have when the copy is executed.
            VkImageLayout image_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

            /// The mip level to copy to.
            u32 mip_level = 0; /// Level 0 is the base image.
        };

        void copy_to_image(const VulkanCommandBuffer& command_buffer, const CopyToImage& copy_to_image) const;

        void generate_mipmap(const VulkanCommandBuffer& command_buffer, const LayoutTransition& layout_transition) const;

        void create_image();

        void destroy_image() const;

        void create_image_view();

        void destroy_image_view() const;

        void allocate_memory();

        void free_memory() const;
    };
}
