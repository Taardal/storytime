#include "st_vulkan_image.h"

#include "graphics/st_vulkan_buffer.h"

#include <stb_image.h>

namespace Storytime {
    VulkanImage::VulkanImage(const Config& config) : config(config) {
        create_image();
        allocate_memory();
        create_image_view();
    }

    VulkanImage::~VulkanImage() {
        destroy_image_view();
        free_memory();
        destroy_image();
    }

    VulkanImage::VulkanImage(VulkanImage&& other) noexcept
        : config(std::move(other.config)),
          image(other.image),
          image_view(other.image_view),
          memory(other.memory)
    {
        other.image = nullptr;
        other.image_view = nullptr;
        other.memory = nullptr;
    }

    VulkanImage& VulkanImage::operator=(VulkanImage&& other) noexcept {
        if (this != &other) {
            config = std::move(other.config);
            image = other.image;
            image_view = other.image_view;
            memory = other.memory;
            other.image = nullptr;
            other.image_view = nullptr;
            other.memory = nullptr;
        }
        return *this;
    }

    VulkanImage::operator VkImage() const {
        return image;
    }

    u32 VulkanImage::get_width() const {
        return config.width;
    }

    u32 VulkanImage::get_height() const {
        return config.height;
    }

    VkImageView VulkanImage::get_view() const {
        return image_view;
    }

    VkFormat VulkanImage::get_format() const {
        return config.format;
    }

    void VulkanImage::set_pixels(const OnRecordCommandsFn& on_record_commands, u64 pixel_data_size, const void* pixel_data) {
        VulkanBuffer staging_buffer({
            .device = config.device,
            .name = std::format("{} staging buffer", config.name),
            .size = pixel_data_size,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        });

        staging_buffer.map_memory();
        staging_buffer.set_data(pixel_data);
        staging_buffer.unmap_memory();

        on_record_commands([&](const VulkanCommandBuffer& command_buffer) {

            // Transition image to be a transfer destination so the buffer can be copied to it.
            transition_layout(command_buffer, LayoutTransition{
                .src_layout = VK_IMAGE_LAYOUT_UNDEFINED,
                .dst_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .src_access = 0, // Nothing to wait on.
                .dst_access = VK_ACCESS_TRANSFER_WRITE_BIT, // The image will be written to.
                .src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, // Nothing to wait on, so start in earlies possible stage.
                .dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT, // Make the image writable by the transfer stage.
                .first_mip_level = 0,
                .mip_level_count = config.mip_levels,
            });

            // Copy the staging buffer to the base image. The pixels will be copied to the mipmaps when they are generated.
            copy_to_image(command_buffer, CopyToImage{
                .buffer = staging_buffer,
                .image_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, // Promise that the image is now in this layout when the copy is executed.
                .mip_level = 0, // Only copy to the base image.
            });
#if 1
            // Generate mipmap, and transition all mip levels in it to be ready to be sampled by the fragment shader.
            generate_mipmap(command_buffer, LayoutTransition{
                .src_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .dst_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .src_access = VK_ACCESS_TRANSFER_WRITE_BIT, // Wait until all writes to this image are complete.
                .dst_access = VK_ACCESS_SHADER_READ_BIT, // The image will be sampled by shaders.
                .src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT, // Wait for writes to complete in the transfer stage.
                .dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, // Make the image visible to fragment shaders.
                .first_mip_level = 0,
                .mip_level_count = config.mip_levels,
            });
#else
            transition_layout(command_buffer, LayoutTransition{
                .src_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .dst_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .src_access = VK_ACCESS_TRANSFER_WRITE_BIT,
                .dst_access = VK_ACCESS_SHADER_READ_BIT,
                .src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT, // Nothing to wait on, so start in earlies possible stage.
                .dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, // Make the image writable by the transfer stage.
                .first_mip_level = 0,
                .mip_level_count = config.mip_levels,
            });
#endif
        });
    }

    void VulkanImage::transition_layout(const VulkanCommandBuffer& command_buffer, const LayoutTransition& layout_transition) const {
        VkImageMemoryBarrier image_memory_barrier{};
        image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        image_memory_barrier.image = image;
        image_memory_barrier.oldLayout = layout_transition.src_layout;
        image_memory_barrier.newLayout = layout_transition.dst_layout;
        image_memory_barrier.srcAccessMask = layout_transition.src_access;
        image_memory_barrier.dstAccessMask = layout_transition.dst_access;
        image_memory_barrier.subresourceRange.aspectMask = config.aspect;
        image_memory_barrier.subresourceRange.baseMipLevel = layout_transition.first_mip_level;
        image_memory_barrier.subresourceRange.levelCount = layout_transition.mip_level_count > 0 ? layout_transition.mip_level_count : config.mip_levels;
        image_memory_barrier.subresourceRange.baseArrayLayer = 0;
        image_memory_barrier.subresourceRange.layerCount = 1;

        command_buffer.pipeline_barrier({
            .src_stage_mask = layout_transition.src_stage,
            .dst_stage_mask = layout_transition.dst_stage,
            .image_memory_barrier_count = 1,
            .image_memory_barriers = &image_memory_barrier,
        });
    }

    void VulkanImage::copy_to_image(const VulkanCommandBuffer& command_buffer, const CopyToImage& copy_to_image) const {
        VkBufferImageCopy copy_region{};
        copy_region.bufferOffset = 0;
        copy_region.bufferRowLength = 0;
        copy_region.bufferImageHeight = 0;
        copy_region.imageOffset = { 0, 0, 0 };
        copy_region.imageExtent = { config.width, config.height, config.depth };
        copy_region.imageSubresource.aspectMask = config.aspect;
        copy_region.imageSubresource.mipLevel = copy_to_image.mip_level;
        copy_region.imageSubresource.baseArrayLayer = 0;
        copy_region.imageSubresource.layerCount = 1;

        command_buffer.copy_buffer_to_image({
            .src_buffer = copy_to_image.buffer,
            .dst_image = image,
            .dst_image_layout = copy_to_image.image_layout, // This is not a transition, it's a promise that the image is already in this layout when the copy is executed.
            .copy_region_count = 1,
            .copy_region = &copy_region,
        });
    }

    // Generate GPU mipmaps by using linear blitting.
    //
    // A "blit" (bit block transfer) is an operation that copies, and possibly resamples, a rectangular region of pixels from one
    // image to another image.
    //
    // Therefore, using "linear blitting" to generate mipmaps means to copy the pixels of the image to every mip level in the mip chain,
    // where the dimensions linearly scales from one mip level to the next (i.e. the every mip level is half the size of the previous level).
    //
    // It should be noted that it is uncommon to generate the mipmap levels at runtime like this. Usually they are pregenerated and stored
    // in the texture file alongside the base level to improve loading speed.
    //
    void VulkanImage::generate_mipmap(const VulkanCommandBuffer& command_buffer, const LayoutTransition& layout_transition) const {

        //
        // Ensure linear blitting is supported.
        //
        // Generating mipmaps by doing linear blitting is not guaranteed to be supported on all platforms, and requires the image's format
        // to support linear filtering.
        //
        // Fallback options:
        // - Use vkCmdCopyImage (no filtering) to generate mipmaps with nearest sampling (bad quality).
        // - Implement mipmap generation in a fragment or compute shader (more flexible and often faster on modern hardware).
        // - Use a compute shader to read from higher mip and write lower mip (can be optimal on many GPUs).
        //

        VkFormatProperties format_properties = config.device->get_physical_device().get_format_properties(config.format);
        bool format_supports_linear_blitting = false;
        if (config.tiling == VK_IMAGE_TILING_LINEAR) {
            format_supports_linear_blitting = format_properties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;
        } else if (config.tiling == VK_IMAGE_TILING_OPTIMAL) {
            format_supports_linear_blitting = format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;
        } else {
            ST_THROW("Could not generate mipmaps for image with invalid tiling [" << get_vk_image_tiling_name(config.tiling) << "]");
        }
        if (!format_supports_linear_blitting) {
            ST_THROW("Could not generate mipmaps for image because format [" << get_vk_format_name(config.format) << "] does not support linear blitting");
        }

        //
        // For each mip level...
        //
        // 1. Transition the SRC mip level layout to be a transfer source so it can be used to create the DST mip level.
        //
        // 2. Use the SRC mip level to "blit" the image to the DST mip level.
        //    - A "blit" (bit block transfer) is an operation that copies, and possibly resamples, a rectangular region of pixels from one
        //      image to another image.
        //
        // 3. Transition the SRC mip level layout to its final layout so it's ready for later use before continuing to the next mip level
        //    in the chain.
        //

        // Determine the starting mip level dimensions.
        i32 src_mip_width = (i32) config.width;
        i32 src_mip_height = (i32) config.height;

        for (u32 i = layout_transition.first_mip_level + 1; i < layout_transition.mip_level_count; i++) {
            u32 src_mip_level = i - 1;
            u32 dst_mip_level = i;

            //
            // Step 1.
            //
            // Transition the SRC mip level layout to be a transfer source so it can be used to create the DST mip level.
            //
            // The pipeline barrier will make the program wait for the SRC mip level to be created, either from a previous blit command,
            // or from copying the staging buffer to the image (the SRC mip level is the base image on the first iteration of the loop).
            //

            transition_layout(command_buffer, {
                .src_layout = layout_transition.src_layout,
                .dst_layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                .src_access = layout_transition.src_access,
                .dst_access = VK_ACCESS_TRANSFER_READ_BIT,
                .src_stage = layout_transition.src_stage,
                .dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT,
                .first_mip_level = src_mip_level,
                .mip_level_count = 1,
            });

            //
            // Step 2.
            //
            // Use the SRC mip level to "blit" the image to the DST mip level.
            //
            // The DST mip level should be half the size of the SRC mip level, or 1. We need to ensure that neither dimension never
            // becomes 0 to handle cases where the image is not square, since one of the mip dimensions would reach 1 before the other
            // dimension. When this happens, that dimension should remain 1 for all remaining levels.
            //

            i32 dst_mip_width = src_mip_width > 1 ? src_mip_width / 2 : 1;
            i32 dst_mip_height = src_mip_height > 1 ? src_mip_height / 2 : 1;

            VkImageBlit image_blit{};
            image_blit.srcOffsets[0] = { 0, 0, 0 };
            image_blit.srcOffsets[1] = { src_mip_width, src_mip_height, (i32) config.depth };
            image_blit.srcSubresource.aspectMask = config.aspect;
            image_blit.srcSubresource.mipLevel = src_mip_level;
            image_blit.srcSubresource.baseArrayLayer = 0;
            image_blit.srcSubresource.layerCount = 1;
            image_blit.dstOffsets[0] = { 0, 0, 0 };
            image_blit.dstOffsets[1] = { dst_mip_width, dst_mip_height, (i32) config.depth };
            image_blit.dstSubresource.aspectMask = config.aspect;
            image_blit.dstSubresource.mipLevel = dst_mip_level;
            image_blit.dstSubresource.baseArrayLayer = 0;
            image_blit.dstSubresource.layerCount = 1;

            command_buffer.blit_image({
                .src_image = image,
                .src_image_layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                .dst_image = image,
                .dst_image_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .image_blit_count = 1,
                .image_blits = &image_blit,
                .filter = VK_FILTER_LINEAR,
            });

            src_mip_width = dst_mip_width;
            src_mip_height = dst_mip_height;

            //
            // Step 3.
            //
            // Transition the SRC mip level layout to its final layout so it's ready for later use before continuing to the next mip level
            // in the chain.
            //

            transition_layout(command_buffer, {
                .src_layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                .dst_layout = layout_transition.dst_layout,
                .src_access = VK_ACCESS_TRANSFER_READ_BIT,
                .dst_access = layout_transition.dst_access,
                .src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT,
                .dst_stage = layout_transition.dst_stage,
                .first_mip_level = src_mip_level,
                .mip_level_count = 1,
            });

        }

        //
        // Transition the LAST mip level layout to its final layout so it's ready for later use before continuing to the next mip level
        // in the chain.
        //
        // This wasn't handled by the loop, since the last mip level is never blitted from.
        //

        transition_layout(command_buffer, {
            .src_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .dst_layout = layout_transition.dst_layout,
            .src_access = VK_ACCESS_TRANSFER_WRITE_BIT,
            .dst_access = layout_transition.dst_access,
            .src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT,
            .dst_stage = layout_transition.dst_stage,
            .first_mip_level = config.mip_levels - 1,
            .mip_level_count = 1,
        });
    }

    void VulkanImage::create_image() {
        const VulkanDevice& device = *config.device;

        VkImageCreateInfo image_create_info{};
        image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.imageType = VK_IMAGE_TYPE_2D;
        image_create_info.extent.width = config.width;
        image_create_info.extent.height = config.height;
        image_create_info.extent.depth = config.depth;
        image_create_info.mipLevels = config.mip_levels;
        image_create_info.arrayLayers = 1;
        image_create_info.format = config.format;
        image_create_info.tiling = config.tiling;
        image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_create_info.usage = config.usage;
        image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
        image_create_info.flags = 0;

        if (device.create_image(image_create_info, &image) != VK_SUCCESS) {
            ST_THROW("Could not create image [" << config.name << "]");
        }

        if (device.set_object_name(image, VK_OBJECT_TYPE_IMAGE, config.name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set image name [" << config.name << "]");
        }
    }

    void VulkanImage::destroy_image() const {
        if (image != nullptr) {
            config.device->destroy_image(image);
        }
    }

    void VulkanImage::create_image_view() {
        VkImageViewCreateInfo image_view_create_info{};
        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.image = image;
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = config.format;
        image_view_create_info.subresourceRange.aspectMask = config.aspect;
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = config.mip_levels;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;

        if (config.device->create_image_view(image_view_create_info, &image_view) != VK_SUCCESS) {
            ST_THROW("Could not create image view for image [" << config.name << "]");
        }

        std::string image_view_name = std::format("{} view", config.name);
        if (config.device->set_object_name(image_view, VK_OBJECT_TYPE_IMAGE_VIEW, image_view_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not create image view name [" << image_view_name << "]");
        }
    }

    void VulkanImage::destroy_image_view() const {
        if (image_view != nullptr) {
            config.device->destroy_image_view(image_view);
        }
    }

    void VulkanImage::allocate_memory() {
        const VulkanDevice& device = *config.device;
        const VulkanPhysicalDevice& physical_device = device.get_physical_device();

        VkMemoryRequirements memory_requirements = device.get_image_memory_requirements(image);
        VkMemoryPropertyFlags required_memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        i32 memory_type_index = physical_device.find_supported_memory_type(memory_requirements, required_memory_properties);

        VkMemoryAllocateInfo memory_allocate_info{};
        memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memory_allocate_info.allocationSize = memory_requirements.size;
        memory_allocate_info.memoryTypeIndex = memory_type_index;

        if (device.allocate_memory(memory_allocate_info, &memory) != VK_SUCCESS) {
            ST_THROW("Could not allocate buffer memory");
        }

        std::string memory_name = std::format("{} memory", config.name);
        if (device.set_object_name(memory, VK_OBJECT_TYPE_DEVICE_MEMORY, memory_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set buffer memory name [" << memory_name << "]");
        }

        if (device.bind_image_memory(image, memory) != VK_SUCCESS) {
            ST_THROW("Could not bind buffer memory");
        }
    }

    void VulkanImage::free_memory() const {
        if (memory != nullptr) {
            config.device->free_memory(memory);
        }
    }
}
