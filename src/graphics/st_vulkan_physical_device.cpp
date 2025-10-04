#include "st_vulkan_physical_device.h"

namespace Storytime {
    VulkanPhysicalDevice::VulkanPhysicalDevice(const Config& config) : config(config) {
        std::vector<VkPhysicalDevice> available_devices = config.instance->get_physical_devices();
        if (available_devices.empty()) {
            ST_THROW("Could not find any available physical device");
        }

        PhysicalDeviceInfo most_suitable_device_info = get_most_suitable_physical_device(available_devices, get_required_extensions());
        if (!most_suitable_device_info.physical_device) {
            ST_THROW("Could not find any suitable physical device");
        }

        ST_LOG_DEBUG("Picked physical device [{}]", most_suitable_device_info.properties.deviceName);
        device_info = most_suitable_device_info;
    }

    VkFormat VulkanPhysicalDevice::get_depth_format() const {
        return device_info.depth_format;
    }

    const std::vector<VkExtensionProperties>& VulkanPhysicalDevice::get_extensions() const {
        return device_info.extensions;
    }

    const VkPhysicalDeviceFeatures& VulkanPhysicalDevice::get_features() const {
        return device_info.features;
    }

    const std::vector<VkPresentModeKHR>& VulkanPhysicalDevice::get_present_modes() const {
        return device_info.present_modes;
    }

    const VkPhysicalDeviceProperties& VulkanPhysicalDevice::get_properties() const {
        return device_info.properties;
    }

    const QueueFamilyIndices& VulkanPhysicalDevice::get_queue_family_indices() const {
        return device_info.queue_family_indices;
    }

    const VkSurfaceCapabilitiesKHR& VulkanPhysicalDevice::get_surface_capabilities() const {
        return device_info.surface_capabilities;
    }

    const std::vector<VkSurfaceFormatKHR>& VulkanPhysicalDevice::get_surface_formats() const {
        return device_info.surface_formats;
    }

    VkResult VulkanPhysicalDevice::create_device(const VkDeviceCreateInfo& device_create_info, VkDevice* device) const {
        return vkCreateDevice(device_info.physical_device, &device_create_info, ST_VK_ALLOCATOR, device);
    }

    VkPhysicalDeviceMemoryProperties VulkanPhysicalDevice::get_memory_properties() const {
        VkPhysicalDeviceMemoryProperties memory_properties;
        vkGetPhysicalDeviceMemoryProperties(device_info.physical_device, &memory_properties);
        return memory_properties;
    }

    void VulkanPhysicalDevice::get_memory_properties(VkPhysicalDeviceMemoryProperties* memory_properties) const {
        vkGetPhysicalDeviceMemoryProperties(device_info.physical_device, memory_properties);
    }

    // Graphics cards can offer different types of memory to allocate from. Each type of memory varies in terms of allowed operations
    // and performance characteristics. This function returns the index memory type that fulfills the given requirements.
    i32 VulkanPhysicalDevice::get_memory_type_index(const VkMemoryRequirements& memory_requirements, VkMemoryPropertyFlags required_memory_properties) const {
        VkPhysicalDeviceMemoryProperties memory_properties = get_memory_properties();

        // The VkPhysicalDeviceMemoryProperties structure has two arrays memoryTypes and memoryHeaps. Memory heaps are distinct memory
        // resources like dedicated VRAM and swap space in RAM for when VRAM runs out. The different types of memory exist within these heaps.
        for (u32 memory_type_index = 0; memory_type_index < memory_properties.memoryTypeCount; memory_type_index++) {

            // Check if the memory type is suitable for the buffer by checking if the corresponding bit is set to 1.
            bool memory_type_is_suitable = (memory_requirements.memoryTypeBits & 1 << memory_type_index) > 0;
            if (!memory_type_is_suitable) {
                continue;
            }

            // Check if the memory type has all required properties by doing a bitwise AND between the candidate and required properties,
            // and checking if the result is not just non-zero, but equal to the required properties bit field.
            //
            // The memoryTypes array consists of VkMemoryType structs that specify the heap and properties of each type of memory.
            // The properties define special features of the memory, like being able to map it so we can write to it from the CPU.
            //
            const VkMemoryType& memory_type = memory_properties.memoryTypes[memory_type_index];
            bool memory_type_has_required_properties = (memory_type.propertyFlags & required_memory_properties) == required_memory_properties;
            if (!memory_type_has_required_properties) {
                continue;
            }

            return memory_type_index;
        }
        return -1;
    }

    std::vector<const char*> VulkanPhysicalDevice::get_required_extensions() const {
        std::vector extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME // Enable VkSwapchainKHR objects so the rendered graphics can be presented to a window surface.
        };
#ifdef ST_PLATFORM_MACOS
        // MacOS does not have native Vulkan drivers, so Vulkan apps have to use the MoltenVK library which layers Vulkan functionality
        // over Apple's native Metal API, translating Vulkan calls into equivalent Metal calls.
        extensions.push_back("VK_KHR_portability_subset");
#endif
        return extensions;
    }

    PhysicalDeviceInfo VulkanPhysicalDevice::get_most_suitable_physical_device(
        const std::vector<VkPhysicalDevice>& available_devices,
        const std::vector<const char*>& required_extensions
    ) const {
        u32 highest_rating = 0;
        PhysicalDeviceInfo highest_rated_device{};
        for (VkPhysicalDevice available_device : available_devices) {
            PhysicalDeviceInfo physical_device_info = find_physical_device_info(available_device, required_extensions);
            u32 suitability_rating = get_suitability_rating(physical_device_info, required_extensions);
            if (suitability_rating > highest_rating) {
                highest_rating = suitability_rating;
                highest_rated_device = physical_device_info;
            }
        }
        return highest_rated_device;
    }

    PhysicalDeviceInfo VulkanPhysicalDevice::find_physical_device_info(
        VkPhysicalDevice physical_device,
        const std::vector<const char*>& required_extensions
    ) const {
        PhysicalDeviceInfo physical_device_info{};
        physical_device_info.physical_device = physical_device;
        physical_device_info.depth_format = find_depth_format(physical_device);
        physical_device_info.extensions = find_extensions(physical_device, required_extensions);
        physical_device_info.features = find_features(physical_device);
        physical_device_info.present_modes = find_present_modes(physical_device);
        physical_device_info.properties = find_properties(physical_device);
        physical_device_info.queue_family_indices = find_queue_family_indices(physical_device);
        physical_device_info.surface_capabilities = find_surface_capabilities(physical_device);
        physical_device_info.surface_formats = find_surface_formats(physical_device);
        return physical_device_info;
    }

    VkFormat VulkanPhysicalDevice::find_depth_format(VkPhysicalDevice physical_device) const {
        std::vector depth_formats = {
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D24_UNORM_S8_UINT
        };
        VkFormatFeatureFlagBits required_features = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
        for (VkFormat depth_format : depth_formats) {
            VkFormatProperties format_properties;
            vkGetPhysicalDeviceFormatProperties(physical_device, depth_format, &format_properties);
            if ((format_properties.optimalTilingFeatures & required_features) == required_features) {
                return depth_format;
            }
        }
        return VK_FORMAT_UNDEFINED;
    }

    std::vector<VkExtensionProperties> VulkanPhysicalDevice::find_extensions(
        VkPhysicalDevice physical_device,
        const std::vector<const char*>& required_extensions
    ) const {
        auto available_extensions = get_available_extensions(physical_device);
        std::vector<VkExtensionProperties> extensions;
        for (const char* required_extension : required_extensions) {
            for (const VkExtensionProperties& available_extension : available_extensions) {
                if (strcmp(required_extension, available_extension.extensionName) == 0) {
                    extensions.push_back(available_extension);
                }
            }
        }
        return extensions;
    }

    std::vector<VkExtensionProperties> VulkanPhysicalDevice::get_available_extensions(VkPhysicalDevice physical_device) const {
        const char* layer_name = nullptr;
        u32 extension_count = 0;
        if (vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, nullptr) != VK_SUCCESS) {
            ST_THROW("Could not get Vulkan physical device extension count");
        }
        std::vector<VkExtensionProperties> extensions(extension_count);
        if (vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, extensions.data()) != VK_SUCCESS) {
            ST_THROW("Could not get Vulkan physical device extensions");
        }
        return extensions;
    }

    VkPhysicalDeviceFeatures VulkanPhysicalDevice::find_features(VkPhysicalDevice physical_device) const {
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physical_device, &features);
        return features;
    }

    std::vector<VkPresentModeKHR> VulkanPhysicalDevice::find_present_modes(VkPhysicalDevice physical_device) const {
        VkSurfaceKHR surface = config.instance->get_surface();
        u32 present_mode_count = 0;
        if (vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr) != VK_SUCCESS) {
            ST_THROW("Could not get Vulkan physical device present mode count");
        }
        std::vector<VkPresentModeKHR> present_modes(present_mode_count);
        if (vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes.data()) != VK_SUCCESS) {
            ST_THROW("Could not get Vulkan physical device present modes");
        }
        return present_modes;
    }

    VkPhysicalDeviceProperties VulkanPhysicalDevice::find_properties(VkPhysicalDevice physical_device) const {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_device, &properties);
        return properties;
    }

    QueueFamilyIndices VulkanPhysicalDevice::find_queue_family_indices(VkPhysicalDevice physical_device) const {
        u32 queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

        QueueFamilyIndices indices;
        for (u32 queue_family_index = 0; queue_family_index < queue_families.size(); queue_family_index++) {
            const VkQueueFamilyProperties& queue_family = queue_families[queue_family_index];
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = queue_family_index;
            }
            VkBool32 presentation_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, queue_family_index, config.instance->get_surface(), &presentation_support);
            if (presentation_support) {
                indices.present_family = queue_family_index;
            }
            if (indices.graphics_family.has_value() && indices.present_family.has_value()) {
                break;
            }
        }
        return indices;
    }

    VkSurfaceCapabilitiesKHR VulkanPhysicalDevice::find_surface_capabilities(VkPhysicalDevice physical_device) const {
        VkSurfaceCapabilitiesKHR surface_capabilities{};
        if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, config.instance->get_surface(), &surface_capabilities) != VK_SUCCESS) {
            ST_THROW("Could not get Vulkan physical device surface capabilities");
        }
        return surface_capabilities;
    }

    std::vector<VkSurfaceFormatKHR> VulkanPhysicalDevice::find_surface_formats(VkPhysicalDevice physical_device) const {
        VkSurfaceKHR surface = config.instance->get_surface();
        u32 format_count = 0;
        if (vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr) != VK_SUCCESS) {
            ST_THROW("Could not get Vulkan physical device surface format count");
        }
        std::vector<VkSurfaceFormatKHR> surface_formats(format_count);
        if (vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, surface_formats.data()) != VK_SUCCESS) {
            ST_THROW("Could not get Vulkan physical device surface formats");
        }
        return surface_formats;
    }

    u32 VulkanPhysicalDevice::get_suitability_rating(
        const PhysicalDeviceInfo& physical_device_info,
        const std::vector<const char*>& required_extensions
    ) const {
        if (!has_required_depth_format(physical_device_info)) {
            ST_LOG_DEBUG("[{}] does not have a suitable depth format", physical_device_info.properties.deviceName);
            return 0;
        }
        if (!has_required_extensions(physical_device_info, required_extensions)) {
            ST_LOG_DEBUG("[{}] does not have required device extensions", physical_device_info.properties.deviceName);
            return 0;
        }
        if (!has_required_features(physical_device_info)) {
            ST_LOG_DEBUG("[{}] does not have required device features", physical_device_info.properties.deviceName);
            return 0;
        }
        if (!has_required_queue_family_indices(physical_device_info)) {
            ST_LOG_DEBUG("[{}] does not have required queue family indices", physical_device_info.properties.deviceName);
            return 0;
        }
        if (!has_required_swapchain_support(physical_device_info)) {
            ST_LOG_DEBUG("[{}] does not have required swap chain info", physical_device_info.properties.deviceName);
            return 0;
        }

        u32 rating = 0;

        // Discrete GPUs have a significant performance advantage.
        if (physical_device_info.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            rating += 1000;
        }

        // Maximum possible size of textures affects graphics quality.
        rating += physical_device_info.properties.limits.maxImageDimension2D;

        return rating;
    }

    bool VulkanPhysicalDevice::has_required_depth_format(const PhysicalDeviceInfo& physical_device_info) const {
        return physical_device_info.depth_format != VK_FORMAT_UNDEFINED;
    }

    bool VulkanPhysicalDevice::has_required_extensions(
        const PhysicalDeviceInfo& physical_device_info,
        const std::vector<const char*>& required_extensions
    ) const {
        for (const char* required_extension : required_extensions) {
            bool extension_found = false;
            for (const VkExtensionProperties& device_extension : physical_device_info.extensions) {
                if (strcmp(required_extension, device_extension.extensionName) == 0) {
                    extension_found = true;
                    break;
                }
            }
            if (!extension_found) {
                ST_LOG_ERROR("Could not find extension [{0}]", required_extension);
                return false;
            }
        }
        return true;
    }

    bool VulkanPhysicalDevice::has_required_features(const PhysicalDeviceInfo& physical_device_info) const {
        return physical_device_info.features.samplerAnisotropy;
    }

    bool VulkanPhysicalDevice::has_required_queue_family_indices(const PhysicalDeviceInfo& physical_device_info) const {
        const QueueFamilyIndices& queue_family_indices = physical_device_info.queue_family_indices;
        return queue_family_indices.graphics_family.has_value() && queue_family_indices.present_family.has_value();
    }

    bool VulkanPhysicalDevice::has_required_swapchain_support(const PhysicalDeviceInfo& physical_device_info) const {
        return !physical_device_info.surface_formats.empty() && !physical_device_info.present_modes.empty();
    }
}
