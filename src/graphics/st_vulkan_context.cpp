#include "st_vulkan_context.h"

namespace Storytime {
    std::string get_debug_message_type_name(VkDebugUtilsMessageTypeFlagsEXT message_type_flags) {
        std::string message_type_name;
        if (message_type_flags & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
            message_type_name += "GENERAL | ";
        }
        if (message_type_flags & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
            message_type_name += "VALIDATION | ";
        }
        if (message_type_flags & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
            message_type_name += "PERFORMANCE | ";
        }
        if (!message_type_name.empty()) {
            message_type_name.erase(message_type_name.size() - 3); // Trim trailing " | "
        }
        return message_type_name;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL on_debug_message(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data
    ) {
        if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            ST_LOG_ERROR("[{}] {}", get_debug_message_type_name(message_type), callback_data->pMessage);
        } else if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            ST_LOG_WARNING("[{}] {}", get_debug_message_type_name(message_type), callback_data->pMessage);
        } else if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            ST_LOG_INFO("[{}] {}", get_debug_message_type_name(message_type), callback_data->pMessage);
        } else {
            ST_LOG_TRACE("[{}] {}", get_debug_message_type_name(message_type), callback_data->pMessage);
        }
        return VK_FALSE;
    }
}

namespace Storytime {
    VulkanContext::VulkanContext(const Config& config) : config(config.assert_valid()) {
        std::vector<const char*> instance_extensions = get_enabled_instance_extensions();
        if (!has_instance_extensions(instance_extensions)) {
            ST_THROW("System does not have enabled Vulkan extensions");
        }

        std::vector<const char*> validation_layers;
        if (config.validation_layers_enabled) {
            validation_layers = get_enabled_validation_layers();
            if (!has_validation_layers(validation_layers)) {
                ST_THROW("System does not have enabled Vulkan validation layers");
            }
        }

        VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{};
        if (config.validation_layers_enabled) {
            debug_messenger_create_info = get_debug_messenger_create_info();
        }

        create_instance(instance_extensions, validation_layers, debug_messenger_create_info);

        if (config.validation_layers_enabled) {
            create_debug_messenger(debug_messenger_create_info);
        }

        create_surface();
    }

    VulkanContext::~VulkanContext() {
        destroy_surface();
        if (config.validation_layers_enabled) {
            destroy_debug_messenger();
        }
        destroy_instance();
    }

    VulkanContext::operator VkInstance() const {
        return instance;
    }

    VkSurfaceKHR VulkanContext::get_surface() const {
        return surface;
    }

    VkResult VulkanContext::get_physical_devices(u32* physical_device_count, VkPhysicalDevice* physical_devices) const {
        return vkEnumeratePhysicalDevices(instance, physical_device_count, physical_devices);
    }

    VkResult VulkanContext::get_physical_devices(std::vector<VkPhysicalDevice>* physical_devices) const {
        u32 physical_device_count = 0;
        VkResult result = vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
        if (result != VK_SUCCESS) {
            return result;
        }
        physical_devices->resize(physical_device_count);
        return vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices->data());
    }

    void VulkanContext::create_instance(
        const std::vector<const char*>& extensions,
        const std::vector<const char*>& validation_layers,
        const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info
    ) {
        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.apiVersion = config.api_version;
        app_info.pApplicationName = config.app_name.c_str();
        app_info.applicationVersion = config.app_version;
        app_info.pEngineName = config.engine_name.c_str();
        app_info.engineVersion = config.engine_version;

        VkInstanceCreateInfo instance_create_info{};
        instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info.pApplicationInfo = &app_info;
        instance_create_info.enabledExtensionCount = extensions.size();
        instance_create_info.ppEnabledExtensionNames = extensions.data();
#ifdef ST_PLATFORM_MACOS
        instance_create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        if (config.validation_layers_enabled) {
            instance_create_info.pNext = &debug_messenger_create_info;
        }
        if (config.validation_layers_enabled) {
            instance_create_info.enabledLayerCount = validation_layers.size();
            instance_create_info.ppEnabledLayerNames = validation_layers.data();
        }

        ST_ASSERT_THROW_VK(
            vkCreateInstance(&instance_create_info, ST_VK_ALLOCATOR, &instance),
            "Could not create Vulkan instance"
        );
    }

    void VulkanContext::destroy_instance() const {
        vkDestroyInstance(instance, ST_VK_ALLOCATOR);
    }

    std::vector<const char*> VulkanContext::get_enabled_instance_extensions() const {
        u32 extension_count = 0;
        const char** extension_names = glfwGetRequiredInstanceExtensions(&extension_count);
        std::vector<const char*> extensions(extension_names, extension_names + extension_count);

#ifdef ST_PLATFORM_MACOS
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif
        if (config.validation_layers_enabled) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    std::vector<VkExtensionProperties> VulkanContext::get_available_instance_extensions() const {
        u32 extension_count = 0;
        const char* layer_name = nullptr;
        VkResult result = vkEnumerateInstanceExtensionProperties(layer_name, &extension_count, nullptr);
        if (result != VK_SUCCESS) {
            ST_THROW("Could not get extension count: " << format_vk_result(result));
        }
        std::vector<VkExtensionProperties> extensions(extension_count);
        result = vkEnumerateInstanceExtensionProperties(layer_name, &extension_count, extensions.data());
        if (result != VK_SUCCESS) {
            ST_THROW("Could not get [" << extensions.size() << "] extensions: " << format_vk_result(result));
        }
        return extensions;
    }

    bool VulkanContext::has_instance_extensions(const std::vector<const char*>& instance_extension_names) const {
        std::vector<VkExtensionProperties> available_instance_extensions = get_available_instance_extensions();
        for (const char* instance_extension_name : instance_extension_names) {
            bool extension_found = false;
            for (const VkExtensionProperties& available_instance_extension: available_instance_extensions) {
                if (strcmp(instance_extension_name, available_instance_extension.extensionName) == 0) {
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

    std::vector<const char*> VulkanContext::get_enabled_validation_layers() const {
        return { "VK_LAYER_KHRONOS_validation" };
    }

    std::vector<VkLayerProperties> VulkanContext::get_available_validation_layers() const {
        u32 validation_layer_count = 0;
        VkLayerProperties* properties = nullptr;
        VkResult result = vkEnumerateInstanceLayerProperties(&validation_layer_count, properties);
        if (result != VK_SUCCESS) {
            ST_THROW("Could not get validation layer count: " << format_vk_result(result));
        }
        std::vector<VkLayerProperties> validation_layers(validation_layer_count);
        result = vkEnumerateInstanceLayerProperties(&validation_layer_count, validation_layers.data());
        if (result != VK_SUCCESS) {
            ST_THROW("Could not get [" << validation_layers.size() << "] validation layers: " << format_vk_result(result));
        }
        return validation_layers;
    }

    bool VulkanContext::has_validation_layers(const std::vector<const char*>& validation_layers) const {
        std::vector<VkLayerProperties> available_validation_layers = get_available_validation_layers();
        for (const char* layer_name: validation_layers) {
            bool layer_found = false;
            for (const auto& availableLayer: available_validation_layers) {
                if (strcmp(layer_name, availableLayer.layerName) == 0) {
                    layer_found = true;
                    break;
                }
            }
            if (!layer_found) {
                return false;
            }
        }
        return true;
    }

    void VulkanContext::create_debug_messenger(const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info) {
        auto create_debug_messenger_fn = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (!create_debug_messenger_fn) {
            ST_THROW("Could not get Vulkan debug messenger create function");
        }
        VkResult result = create_debug_messenger_fn(instance, &debug_messenger_create_info, ST_VK_ALLOCATOR, &debug_messenger);
        if (result != VK_SUCCESS) {
            ST_THROW("Could not create Vulkan debug messenger: " << format_vk_result(result));
        }
    }

    void VulkanContext::destroy_debug_messenger() const {
        auto destroy_debug_messenger_fn = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (!destroy_debug_messenger_fn) {
            ST_THROW("Could not get Vulkan debug messenger destroy function");
        }
        destroy_debug_messenger_fn(instance, debug_messenger, ST_VK_ALLOCATOR);
    }

    VkDebugUtilsMessengerCreateInfoEXT VulkanContext::get_debug_messenger_create_info() const {
        VkDebugUtilsMessengerCreateInfoEXT create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        create_info.pfnUserCallback = on_debug_message;
        return create_info;
    }

    void VulkanContext::create_surface() {
        ST_ASSERT_THROW_VK(
            glfwCreateWindowSurface(instance, config.window, ST_VK_ALLOCATOR, &surface),
            "Could not create Vulkan surface"
        );
    }

    void VulkanContext::destroy_surface() const {
        vkDestroySurfaceKHR(instance, surface, ST_VK_ALLOCATOR);
    }
}
