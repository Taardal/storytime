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

    VkImageView VulkanImage::get_view() const {
        return image_view;
    }

    VkFormat VulkanImage::get_format() const {
        return config.format;
    }

    void VulkanImage::set_data(const void* data, u64 data_size, const OnRecordCommandsFn& on_record_commands) {
        VulkanBuffer staging_buffer({
            .device = config.device,
            .name = std::format("{} staging buffer", config.name),
            .size = data_size,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        });

        staging_buffer.map_memory();
        staging_buffer.set_data(data);
        staging_buffer.unmap_memory();

        on_record_commands([&](const VulkanCommandBuffer& command_buffer) {
            set_layout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, command_buffer);
            copy_to_image(staging_buffer, command_buffer);
            set_layout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, command_buffer);
        });
    }

    void VulkanImage::set_layout(VkImageLayout layout, const VulkanCommandBuffer& command_buffer) {
        VkImageLayout old_layout = config.layout;
        VkImageLayout new_layout = layout;

        // Memory access masks:
        // - Specify which types of memory operations must be synchronized.
        // - Used to declare hazards that must be resolved before/after the transition.
        VkAccessFlags src_access; // Operations to wait on before the transition (previous accesses).
        VkAccessFlags dst_access; // Operations that will use the image after the transition (next accesses).

        // Pipeline stage masks:
        // - Specify where in the GPU pipeline the synchronization occurs.
        // - Define the scope of the access masks: i.e., *when* those reads/writes are relevant.
        VkPipelineStageFlags src_stage; // Earliest pipeline stage that could have performed `src_access` operations.
        VkPipelineStageFlags dst_stage; // Earliest pipeline stage that will perform `dst_access` operations.

        // [Undefined → Transfer destination]
        // Transition for uploading data into the image.
        if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            src_access = 0; // Nothing to wait on.
            src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT; // Nothing to wait on, so start in earlies possible stage.
            dst_access = VK_ACCESS_TRANSFER_WRITE_BIT; // The image will be written to.
            dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT; // Make the image writable by the transfer stage.

        // [Transfer → Shader read]
        // Transition for preparing the image for shader reads after uploading data into the image.
        } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            src_access = VK_ACCESS_TRANSFER_WRITE_BIT; // Wait until all writes to this image are complete.
            src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT; // Wait for writes to complete in the transfer stage.
            dst_access = VK_ACCESS_SHADER_READ_BIT; // The image will be read (sampled) by shaders.
            dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT; // Make the image visible to fragment shaders reads (sampling).

        // All other transitions are unsupported.
        } else {
            ST_THROW("Unsupported image layout transition: [" << get_vk_image_layout_name(layout) << "] -> [" << get_vk_image_layout_name(new_layout) << "]");
        }

        // Use a pipeline barrier to perform the layout transition.
        // - Ensure all reads/writes to the old layout finish before starting any access in the new layout.
        // - Change the layout metadata so that subsequent accesses interpret the image correctly.
        VkImageMemoryBarrier image_memory_barrier{};
        image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        image_memory_barrier.oldLayout = old_layout;
        image_memory_barrier.newLayout = new_layout;
        image_memory_barrier.image = image;
        image_memory_barrier.srcAccessMask = src_access;
        image_memory_barrier.dstAccessMask = dst_access;
        image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // Do not transfer queue family ownership.
        image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // Do not transfer queue family ownership.
        image_memory_barrier.subresourceRange.aspectMask = config.aspect;
        image_memory_barrier.subresourceRange.baseMipLevel = 0;
        image_memory_barrier.subresourceRange.levelCount = 1;
        image_memory_barrier.subresourceRange.baseArrayLayer = 0;
        image_memory_barrier.subresourceRange.layerCount = 1;

        command_buffer.pipeline_barrier({
            .src_stage_mask = src_stage,
            .dst_stage_mask = dst_stage,
            .dependency_flags = 0,
            .memory_barrier_count = 0,
            .memory_barriers = nullptr,
            .buffer_memory_barrier_count = 0,
            .buffer_memory_barriers = nullptr,
            .image_memory_barrier_count = 1,
            .image_memory_barriers = &image_memory_barrier,
        });

        config.layout = new_layout;
    }

    void VulkanImage::copy_to_image(const VulkanBuffer& buffer, const VulkanCommandBuffer& command_buffer) const {
        VkOffset3D image_offset{};
        image_offset.x = 0;
        image_offset.y = 0;
        image_offset.z = 0;

        VkExtent3D image_extent{};
        image_extent.width = config.width;
        image_extent.height = config.height;
        image_extent.depth = 1;

        VkBufferImageCopy copy_region{};
        copy_region.bufferOffset = 0;
        copy_region.bufferRowLength = 0;
        copy_region.bufferImageHeight = 0;
        copy_region.imageSubresource.aspectMask = config.aspect;
        copy_region.imageSubresource.mipLevel = 0;
        copy_region.imageSubresource.baseArrayLayer = 0;
        copy_region.imageSubresource.layerCount = 1;
        copy_region.imageOffset = image_offset;
        copy_region.imageExtent = image_extent;

        command_buffer.copy_buffer_to_image({
            .src_buffer = buffer,
            .dst_image = image,
            .dst_image_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .copy_region_count = 1,
            .copy_region = &copy_region,
        });
    }

    void VulkanImage::create_image() {
        const VulkanDevice& device = *config.device;

        VkImageCreateInfo image_create_info{};
        image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.imageType = VK_IMAGE_TYPE_2D;
        image_create_info.extent.width = config.width;
        image_create_info.extent.height = config.height;
        image_create_info.extent.depth = 1;
        image_create_info.mipLevels = 1;
        image_create_info.arrayLayers = 1;
        image_create_info.format = config.format;
        image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
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
        image_view_create_info.subresourceRange.levelCount = 1;
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
