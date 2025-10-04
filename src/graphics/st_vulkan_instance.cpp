#include "st_vulkan_instance.h"

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
    VulkanInstance::VulkanInstance(const Config& config) : config(config) {
        std::vector<const char*> extensions = get_required_extensions();
        if (!has_extensions(extensions)) {
            ST_THROW("System does not have required Vulkan extensions");
        }

        std::vector<const char*> validation_layers;
        if (config.validation_layers_enabled) {
            validation_layers = get_required_validation_layers();
            if (!has_validation_layers(validation_layers)) {
                ST_THROW("System does not have required Vulkan validation layers");
            }
        }

        VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{};
        if (config.validation_layers_enabled) {
            debug_messenger_create_info = get_debug_messenger_create_info();
        }

        create_instance(extensions, validation_layers, debug_messenger_create_info);

        if (config.validation_layers_enabled) {
            create_debug_messenger(debug_messenger_create_info);
        }

        create_surface();
    }

    VulkanInstance::~VulkanInstance() {
        destroy_surface();
        if (config.validation_layers_enabled) {
            destroy_debug_messenger();
        }
        destroy_instance();
    }

    VulkanInstance::operator VkInstance() const {
        return instance;
    }

    VkSurfaceKHR VulkanInstance::get_surface() const {
        return surface;
    }

    std::vector<VkPhysicalDevice> VulkanInstance::get_physical_devices() const {
        u32 device_count = 0;
        if (vkEnumeratePhysicalDevices(instance, &device_count, nullptr) != VK_SUCCESS) {
            ST_THROW("Could not get physical device count");
        }
        std::vector<VkPhysicalDevice> devices(device_count);
        if (vkEnumeratePhysicalDevices(instance, &device_count, devices.data()) != VK_SUCCESS) {
            ST_THROW("Could not get physical devices");
        }
        return devices;
    }

    void VulkanInstance::create_instance(
        const std::vector<const char*>& extensions,
        const std::vector<const char*>& validation_layers,
        const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info
    ) {
        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = config.app_name.c_str();
        app_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        app_info.pEngineName = config.engine_name.c_str();
        app_info.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_3;

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

        if (vkCreateInstance(&instance_create_info, ST_VK_ALLOCATOR, &instance) != VK_SUCCESS) {
            ST_THROW("Could not create Vulkan instance");
        }
    }

    void VulkanInstance::destroy_instance() const {
        vkDestroyInstance(instance, ST_VK_ALLOCATOR);
    }

    std::vector<const char*> VulkanInstance::get_required_extensions() const {
        u32 glfw_extension_count = 0;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        std::vector extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
#ifdef ST_PLATFORM_MACOS
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif
        if (config.validation_layers_enabled) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        return extensions;
    }

    std::vector<VkExtensionProperties> VulkanInstance::get_available_extensions() const {
        u32 extension_count = 0;
        const char* layer_name = nullptr;
        if (vkEnumerateInstanceExtensionProperties(layer_name, &extension_count, nullptr) != VK_SUCCESS) {
            ST_THROW("Could not get Vulkan extension count");
        }
        std::vector<VkExtensionProperties> extensions(extension_count);
        if (vkEnumerateInstanceExtensionProperties(layer_name, &extension_count, extensions.data()) != VK_SUCCESS) {
            ST_THROW("Could not get Vulkan extensions");
        }
        return extensions;
    }

    bool VulkanInstance::has_extensions(const std::vector<const char*>& extensions) const {
        std::vector<VkExtensionProperties> available_extensions = get_available_extensions();
        for (const char* extension: extensions) {
            bool extensionFound = false;
            for (const VkExtensionProperties& availableExtension: available_extensions) {
                if (strcmp(extension, availableExtension.extensionName) == 0) {
                    extensionFound = true;
                    break;
                }
            }
            if (!extensionFound) {
                std::cerr << "Could not find extension " << extension << std::endl;
                return false;
            }
        }
        return true;
    }

    std::vector<const char*> VulkanInstance::get_required_validation_layers() const {
        return { "VK_LAYER_KHRONOS_validation" };
    }

    std::vector<VkLayerProperties> VulkanInstance::get_available_validation_layers() const {
        u32 count = 0;
        VkLayerProperties* properties = nullptr;
        if (vkEnumerateInstanceLayerProperties(&count, properties) != VK_SUCCESS) {
            ST_THROW("Could not get Vulkan extension count");
        }
        std::vector<VkLayerProperties> layers(count);
        if (vkEnumerateInstanceLayerProperties(&count, layers.data()) != VK_SUCCESS) {
            ST_THROW("Could not get Vulkan extensions");
        }
        return layers;
    }

    bool VulkanInstance::has_validation_layers(const std::vector<const char*>& validation_layers) const {
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
                std::cerr << "Could not find validation layer " << layer_name << std::endl;
                return false;
            }
        }
        return true;
    }

    void VulkanInstance::create_debug_messenger(const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info) {
        auto create_debug_messenger_fn = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (!create_debug_messenger_fn) {
            ST_THROW("Could not get Vulkan debug messenger create function");
        }
        if (create_debug_messenger_fn(instance, &debug_messenger_create_info, ST_VK_ALLOCATOR, &debug_messenger) != VK_SUCCESS) {
            ST_THROW("Could not create Vulkan debug messenger");
        }
    }

    void VulkanInstance::destroy_debug_messenger() const {
        auto destroy_debug_messenger_fn = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (!destroy_debug_messenger_fn) {
            ST_THROW("Could not get Vulkan debug messenger destroy function");
        }
        destroy_debug_messenger_fn(instance, debug_messenger, ST_VK_ALLOCATOR);
    }

    VkDebugUtilsMessengerCreateInfoEXT VulkanInstance::get_debug_messenger_create_info() const {
        VkDebugUtilsMessengerCreateInfoEXT create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                  | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                  | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        create_info.pfnUserCallback = on_debug_message;
        return create_info;
    }

    void VulkanInstance::create_surface() {
        if (glfwCreateWindowSurface(instance, *config.window, ST_VK_ALLOCATOR, &surface) != VK_SUCCESS) {
            ST_THROW("Could not create Vulkan surface");
        }
    }

    void VulkanInstance::destroy_surface() const {
        vkDestroySurfaceKHR(instance, surface, ST_VK_ALLOCATOR);
    }
}
