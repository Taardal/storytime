#include "st_vulkan_physical_device.h"

namespace Storytime {
    const std::vector<const char*> VulkanPhysicalDevice::enabled_extensions = get_enabled_device_extensions();

    VulkanPhysicalDevice::VulkanPhysicalDevice(const Config& config)
        : config(config),
          surface(config.context->get_surface()),
          physical_device(pick_physical_device()),
          queue_family_indices(find_queue_family_indices(physical_device))
    {
        ST_LOG_I("Using graphics card [{}]", get_properties().deviceName);
    }

    VulkanPhysicalDevice::operator VkPhysicalDevice() const {
        return physical_device;
    }

    u32 VulkanPhysicalDevice::get_graphics_queue_family_index() const {
        return queue_family_indices.graphics_family.value();
    }

    u32 VulkanPhysicalDevice::get_present_queue_family_index() const {
        return queue_family_indices.present_family.value();
    }

    VkResult VulkanPhysicalDevice::create_device(const VkDeviceCreateInfo& device_create_info, VkDevice* device) const {
        return vkCreateDevice(physical_device, &device_create_info, ST_VK_ALLOCATOR, device);
    }

    void VulkanPhysicalDevice::get_features(VkPhysicalDeviceFeatures* features) const {
        vkGetPhysicalDeviceFeatures(physical_device, features);
    }

    VkPhysicalDeviceFeatures VulkanPhysicalDevice::get_features() const {
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physical_device, &features);
        return features;
    }

    void VulkanPhysicalDevice::get_properties(VkPhysicalDeviceProperties* properties) const {
        vkGetPhysicalDeviceProperties(physical_device, properties);
    }

    VkPhysicalDeviceProperties VulkanPhysicalDevice::get_properties() const {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_device, &properties);
        return properties;
    }

    void VulkanPhysicalDevice::get_memory_properties(VkPhysicalDeviceMemoryProperties* memory_properties) const {
        vkGetPhysicalDeviceMemoryProperties(physical_device, memory_properties);
    }

    VkPhysicalDeviceMemoryProperties VulkanPhysicalDevice::get_memory_properties() const {
        VkPhysicalDeviceMemoryProperties memory_properties;
        vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);
        return memory_properties;
    }

    void VulkanPhysicalDevice::get_format_properties(VkFormat format, VkFormatProperties* format_properties) const {
        vkGetPhysicalDeviceFormatProperties(physical_device, format, format_properties);
    }

    VkFormatProperties VulkanPhysicalDevice::get_format_properties(VkFormat format) const {
        VkFormatProperties format_properties;
        vkGetPhysicalDeviceFormatProperties(physical_device, format, &format_properties);
        return format_properties;
    }

    void VulkanPhysicalDevice::get_queue_family_properties(u32* queue_family_property_count, VkQueueFamilyProperties* queue_family_properties) const {
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, queue_family_property_count, queue_family_properties);
    }

    std::vector<VkQueueFamilyProperties> VulkanPhysicalDevice::get_queue_family_properties() const {
        u32 queue_family_property_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_property_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_property_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_property_count, queue_family_properties.data());

        return queue_family_properties;
    }

    VkResult VulkanPhysicalDevice::get_extensions(u32* extension_count, VkExtensionProperties* extensions, const char* layer_name) const {
        return vkEnumerateDeviceExtensionProperties(physical_device, layer_name, extension_count, extensions);
    }

    VkResult VulkanPhysicalDevice::get_extensions(std::vector<VkExtensionProperties>* extensions, const char* layer_name) const {
        u32 extension_count = 0;
        VkResult result = vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, nullptr);
        if (result != VK_SUCCESS) {
            return result;
        }
        extensions->resize(extension_count);
        return vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, extensions->data());
    }

    VkResult VulkanPhysicalDevice::get_present_modes(u32* present_mode_count, VkPresentModeKHR* present_modes) const {
        return vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, present_mode_count, present_modes);
    }

    VkResult VulkanPhysicalDevice::get_present_modes(std::vector<VkPresentModeKHR>* present_modes) const {
        u32 present_mode_count = 0;
        VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
        if (result != VK_SUCCESS) {
            return result;
        }
        present_modes->resize(present_mode_count);
        return vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes->data());
    }

    VkResult VulkanPhysicalDevice::get_surface_capabilities(VkSurfaceCapabilitiesKHR* surface_capabilities) const {
        return vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, surface_capabilities);
    }

    VkResult VulkanPhysicalDevice::get_surface_formats(u32* surface_format_count, VkSurfaceFormatKHR* surface_formats) const {
        return vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, surface_format_count, surface_formats);
    }

    VkResult VulkanPhysicalDevice::get_surface_formats(std::vector<VkSurfaceFormatKHR>* surface_formats) const {
        u32 surface_format_count = 0;
        VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, nullptr);
        if (result != VK_SUCCESS) {
            return result;
        }
        surface_formats->resize(surface_format_count);
        return vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, surface_formats->data());
    }

    VkResult VulkanPhysicalDevice::get_surface_support(u32 queue_family_index, VkSurfaceKHR surface, VkBool32* supported) const {
        return vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, queue_family_index, surface, supported);
    }

    // Graphics cards can offer different types of memory to allocate from. Each type of memory varies in terms of allowed operations
    // and performance characteristics. This function returns the index of the memory type that fulfills the given requirements.
    i32 VulkanPhysicalDevice::find_supported_memory_type(const VkMemoryRequirements& memory_requirements, VkMemoryPropertyFlags required_memory_properties) const {
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

    VkFormat VulkanPhysicalDevice::find_supported_format(const std::vector<VkFormat>& format_candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const {
        for (VkFormat format : format_candidates) {
            VkFormatProperties format_properties = get_format_properties(format);
            if (tiling == VK_IMAGE_TILING_LINEAR && (format_properties.linearTilingFeatures & features) == features) {
                return format;
            }
            if (tiling == VK_IMAGE_TILING_OPTIMAL && (format_properties.optimalTilingFeatures & features) == features) {
                return format;
            }
        }
        return VK_FORMAT_UNDEFINED;
    }

    VkPhysicalDevice VulkanPhysicalDevice::pick_physical_device() const {
        std::vector<VkPhysicalDevice> available_physical_devices;
        if (config.context->get_physical_devices(&available_physical_devices) != VK_SUCCESS) {
            ST_THROW("Could not get physical devices");
        }
        if (available_physical_devices.empty()) {
            ST_THROW("Could not find any available physical device");
        }

        u32 highest_rating = 0;
        VkPhysicalDevice highest_rated_device = nullptr;
        for (VkPhysicalDevice physical_device : available_physical_devices) {
            u32 rating = rate_physical_device(physical_device);
            if (rating > highest_rating) {
                highest_rating = rating;
                highest_rated_device = physical_device;
            }
        }

        return highest_rated_device;
    }

    u32 VulkanPhysicalDevice::rate_physical_device(VkPhysicalDevice physical_device) const {
        if (!has_required_extensions(physical_device)) {
            return 0;
        }
        if (!has_required_queue_family_indices(physical_device)) {
            return 0;
        }
        if (!has_required_features(physical_device)) {
            return 0;
        }
        if (!has_required_swapchain_support(physical_device)) {
            return 0;
        }

        u32 rating = 0;

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_device, &properties);

        // Discrete GPUs have a significant performance advantage.
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            rating += 1000;
        }

        // Maximum possible size of textures affects graphics quality.
        rating += properties.limits.maxImageDimension2D;

        return rating;
    }

    bool VulkanPhysicalDevice::has_required_extensions(VkPhysicalDevice physical_device) const {
        return has_device_extensions(physical_device, enabled_extensions);
    }

    bool VulkanPhysicalDevice::has_required_queue_family_indices(VkPhysicalDevice physical_device) const {
        QueueFamilyIndices queue_family_indices = find_queue_family_indices(physical_device);
        if (!queue_family_indices.graphics_family.has_value()) {
            return false;
        }
        if (!queue_family_indices.present_family.has_value()) {
            return false;
        }
        return true;
    }

    bool VulkanPhysicalDevice::has_required_features(VkPhysicalDevice physical_device) const {
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physical_device, &features);
        return features.samplerAnisotropy;
    }

    bool VulkanPhysicalDevice::has_required_swapchain_support(VkPhysicalDevice physical_device) const {
        u32 present_mode_count = 0;
        if (vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr) != VK_SUCCESS) {
            ST_THROW("Could not get physical device present mode count");
        }
        std::vector<VkPresentModeKHR> present_modes(present_mode_count);
        if (vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes.data()) != VK_SUCCESS) {
            ST_THROW("Could not get physical device present modes");
        }
        if (present_modes.empty()) {
            return false;
        }

        u32 format_count = 0;
        if (vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr) != VK_SUCCESS) {
            ST_THROW("Could not get physical device surface format count");
        }
        std::vector<VkSurfaceFormatKHR> surface_formats(format_count);
        if (vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, surface_formats.data()) != VK_SUCCESS) {
            ST_THROW("Could not get physical device surface formats");
        }
        if (surface_formats.empty()) {
            return false;
        }

        return true;
    }

    VulkanPhysicalDevice::QueueFamilyIndices VulkanPhysicalDevice::find_queue_family_indices(VkPhysicalDevice physical_device) const {
        u32 queue_family_property_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_property_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_property_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_property_count, queue_family_properties.data());

        QueueFamilyIndices queue_family_indices{};
        for (u32 queue_family_index = 0; queue_family_index < queue_family_properties.size(); queue_family_index++) {

            if (queue_family_properties[queue_family_index].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queue_family_indices.graphics_family = queue_family_index;
            }

            VkBool32 can_present_to_surface = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, queue_family_index, surface, &can_present_to_surface);
            if (can_present_to_surface) {
                queue_family_indices.present_family = queue_family_index;
            }

            if (queue_family_indices.graphics_family.has_value() && queue_family_indices.present_family.has_value()) {
                break;
            }
        }
        return queue_family_indices;
    }

    std::vector<const char*> VulkanPhysicalDevice::get_enabled_device_extensions() {
        return {
            // Enable VkSwapchainKHR objects so the rendered graphics can be presented to a window surface.
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,

#ifdef ST_PLATFORM_MACOS
            // MacOS does not have native Vulkan drivers, so Vulkan apps have to use the MoltenVK library which layers Vulkan functionality
            // over Apple's native Metal API, translating Vulkan calls into equivalent Metal calls.
            "VK_KHR_portability_subset",
#endif
        };
    }

    std::vector<VkExtensionProperties> VulkanPhysicalDevice::get_available_device_extensions(VkPhysicalDevice physical_device) {
        const char* layer_name = nullptr;
        u32 extension_count = 0;
        if (vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, nullptr) != VK_SUCCESS) {
            ST_THROW("Could not get available device extension count");
        }
        std::vector<VkExtensionProperties> extensions(extension_count);
        if (vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, extensions.data()) != VK_SUCCESS) {
            ST_THROW("Could not get available device extensions");
        }
        return extensions;
    }

    bool VulkanPhysicalDevice::has_device_extensions(VkPhysicalDevice physical_device, const std::vector<const char*>& device_extension_names) {
        std::vector<VkExtensionProperties> available_extensions = get_available_device_extensions(physical_device);
        for (const char* device_extension_name : device_extension_names) {
            bool extension_found = false;
            for (const VkExtensionProperties& available_extension : available_extensions) {
                if (strcmp(device_extension_name, available_extension.extensionName) == 0) {
                    extension_found = true;
                    break;
                }
            }
            if (!extension_found) {
                return false;
            }
        }
        return true;
    }
}
