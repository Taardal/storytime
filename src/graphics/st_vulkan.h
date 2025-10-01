#pragma once

#include <vulkan/vulkan.h>

#define ST_VK_ALLOCATOR nullptr

// #define ST_ASSERT_VK(expression, message) ST_ASSERT(expression == VK_SUCCESS, message)
// #define ST_ASSERT_THROW_VK(expression, message) ST_ASSERT_THROW(expression == VK_SUCCESS, message)

#define ST_THROW_VK(vk_result, message)\
    ST_THROW(message << ": [" << get_vk_result_name(vk_result) << "] " << get_vk_result_description(vk_result))

#define ST_ASSERT_THROW_VK(expression, message)\
    {\
        VkResult result = expression;\
        if (result == VK_SUCCESS) {\
            /* Continue */\
        } else {\
            ST_THROW(message << ": [" << get_vk_result_name(result) << "] " << get_vk_result_description(result));\
        }\
    }

#ifdef ST_ENABLE_ASSERT
    #define ST_ASSERT_VK(expression, message)\
        {\
            VkResult result = expression;\
            if (result == VK_SUCCESS) {\
                /* Continue */\
            } else {\
                std::stringstream ss;\
                ss << message << ": [" << get_vk_result_name(result) << "] " << get_vk_result_description(result);\
                std::string message_string = ss.str();\
                ST_LOG_CRITICAL("Assertion failed: {} == VK_SUCCESS, {}", #expression, message_string); \
                ST_BREAK(); \
            }\
        }
#endif

namespace Storytime {
    inline std::string get_vk_result_name(VkResult result) {
        switch (result) {
            case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
                return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
            case VK_ERROR_DEVICE_LOST:
                return "VK_ERROR_DEVICE_LOST";
            case VK_ERROR_EXTENSION_NOT_PRESENT:
                return "VK_ERROR_EXTENSION_NOT_PRESENT";
            case VK_ERROR_FEATURE_NOT_PRESENT:
                return "VK_ERROR_FEATURE_NOT_PRESENT";
            case VK_ERROR_FORMAT_NOT_SUPPORTED:
                return "VK_ERROR_FORMAT_NOT_SUPPORTED";
            case VK_ERROR_FRAGMENTATION:
                return "VK_ERROR_FRAGMENTATION";
            case VK_ERROR_FRAGMENTED_POOL:
                return "VK_ERROR_FRAGMENTED_POOL";
            case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
                return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
            case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
                return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
            case VK_ERROR_INCOMPATIBLE_DRIVER:
                return "VK_ERROR_INCOMPATIBLE_DRIVER";
            case VK_ERROR_INITIALIZATION_FAILED:
                return "VK_ERROR_INITIALIZATION_FAILED";
            case VK_ERROR_INVALID_EXTERNAL_HANDLE:
                return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
            case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
                return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
            case VK_ERROR_INVALID_SHADER_NV:
                return "VK_ERROR_INVALID_SHADER_NV";
            case VK_ERROR_LAYER_NOT_PRESENT:
                return "VK_ERROR_LAYER_NOT_PRESENT";
            case VK_ERROR_MEMORY_MAP_FAILED:
                return "VK_ERROR_MEMORY_MAP_FAILED";
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
                return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
            case VK_ERROR_OUT_OF_DATE_KHR:
                return "VK_ERROR_OUT_OF_DATE_KHR";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
            case VK_ERROR_OUT_OF_HOST_MEMORY:
                return "VK_ERROR_OUT_OF_HOST_MEMORY";
            case VK_ERROR_OUT_OF_POOL_MEMORY:
                return "VK_ERROR_OUT_OF_POOL_MEMORY";
            case VK_ERROR_SURFACE_LOST_KHR:
                return "VK_ERROR_SURFACE_LOST_KHR";
            case VK_ERROR_TOO_MANY_OBJECTS:
                return "VK_ERROR_TOO_MANY_OBJECTS";
            case VK_ERROR_UNKNOWN:
                return "VK_ERROR_UNKNOWN";
            case VK_ERROR_VALIDATION_FAILED_EXT:
                return "VK_ERROR_VALIDATION_FAILED_EXT";
            case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
                return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
            case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
                return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
            case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
                return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
            case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
                return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
            case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
                return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
            case VK_EVENT_RESET:
                return "VK_EVENT_RESET";
            case VK_EVENT_SET:
                return "VK_EVENT_SET";
            case VK_INCOMPLETE:
                return "VK_INCOMPLETE";
            case VK_NOT_READY:
                return "VK_NOT_READY";
            case VK_PIPELINE_COMPILE_REQUIRED:
                return "VK_PIPELINE_COMPILE_REQUIRED";
            case VK_SUBOPTIMAL_KHR:
                return "VK_SUBOPTIMAL_KHR";
            case VK_SUCCESS:
                return "VK_SUCCESS";
            case VK_TIMEOUT:
                return "VK_TIMEOUT";
            default:
                return "";
        }
    }

    inline std::string get_vk_result_description(VkResult result) {
        switch (result) {
            case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
                return "Image compression has exhausted internal resources.";
            case VK_ERROR_DEVICE_LOST:
                return "The logical or physical device has been lost.";
            case VK_ERROR_EXTENSION_NOT_PRESENT:
                return "A requested extension is not supported.";
            case VK_ERROR_FEATURE_NOT_PRESENT:
                return "A requested feature is not supported.";
            case VK_ERROR_FORMAT_NOT_SUPPORTED:
                return "A requested format is not supported.";
            case VK_ERROR_FRAGMENTATION:
                return "A descriptor pool creation has failed due to fragmentation.";
            case VK_ERROR_FRAGMENTED_POOL:
                return "A pool allocation has failed due to fragmentation.";
            case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
                return "The requested image usage is not supported.";
            case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
                return "The display used is incompatible with the swapchain.";
            case VK_ERROR_INCOMPATIBLE_DRIVER:
                return "The requested version of Vulkan is not supported by the driver.";
            case VK_ERROR_INITIALIZATION_FAILED:
                return "Initialization of an object could not be completed.";
            case VK_ERROR_INVALID_EXTERNAL_HANDLE:
                return "An external handle is not a valid handle of the specified type.";
            case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
                return "A buffer creation failed because the requested address is not available.";
            case VK_ERROR_INVALID_SHADER_NV:
                return "One or more shaders failed to compile or link.";
            case VK_ERROR_LAYER_NOT_PRESENT:
                return "A requested layer is not present or could not be loaded.";
            case VK_ERROR_MEMORY_MAP_FAILED:
                return "Mapping of a memory object has failed.";
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
                return "The requested window is already connected to another VkSurfaceKHR.";
            case VK_ERROR_OUT_OF_DATE_KHR:
                return "A surface has changed and the swapchain is no longer compatible.";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                return "A device memory allocation has failed.";
            case VK_ERROR_OUT_OF_HOST_MEMORY:
                return "A host memory allocation has failed.";
            case VK_ERROR_OUT_OF_POOL_MEMORY:
                return "A pool memory allocation has failed.";
            case VK_ERROR_SURFACE_LOST_KHR:
                return "A surface is no longer available.";
            case VK_ERROR_TOO_MANY_OBJECTS:
                return "Too many objects of the type have already been created.";
            case VK_ERROR_UNKNOWN:
                return "An unknown error has occurred.";
            case VK_ERROR_VALIDATION_FAILED_EXT:
                return "A validation layer found an error.";
            case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
                return "The requested video picture layout is not supported.";
            case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
                return "The video codec is not supported with the requested profile.";
            case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
                return "The video profile format is not supported.";
            case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
                return "The video profile operation is not supported.";
            case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
                return "The requested video std version is not supported.";
            case VK_EVENT_RESET:
                return "An event is unsignaled.";
            case VK_EVENT_SET:
                return "An event is signaled.";
            case VK_INCOMPLETE:
                return "A return array was too small for the result.";
            case VK_NOT_READY:
                return "A fence or query has not yet completed.";
            case VK_PIPELINE_COMPILE_REQUIRED:
                return "A requested pipeline creation would need to compile but the operation was not requested.";
            case VK_SUBOPTIMAL_KHR:
                return "A swapchain no longer matches the surface properties exactly, but can still be used.";
            case VK_SUCCESS:
                return "Command successfully completed.";
            case VK_TIMEOUT:
                return "A wait operation has not completed in the specified time.";
            default:
                return "";
        }
    }

    inline VkFormat get_vk_format_from_glsl_type(std::string_view type) {
        if (type == "float") {
            return VK_FORMAT_R32_SFLOAT;
        }
        if (type == "vec2") {
            return VK_FORMAT_R32G32_SFLOAT;
        }
        if (type == "vec3") {
            return VK_FORMAT_R32G32B32_SFLOAT;
        }
        if (type == "vec4") {
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        if (type == "double") {
            return VK_FORMAT_R64_SFLOAT;
        }
        if (type == "dvec2") {
            return VK_FORMAT_R64G64_SFLOAT;
        }
        if (type == "dvec3") {
            return VK_FORMAT_R64G64B64_SFLOAT;
        }
        if (type == "dvec4") {
            return VK_FORMAT_R64G64B64A64_SFLOAT;
        }
        if (type == "int") {
            return VK_FORMAT_R32_SINT;
        }
        if (type == "ivec2") {
            return VK_FORMAT_R32G32_SINT;
        }
        if (type == "ivec3") {
            return VK_FORMAT_R32G32B32_SINT;
        }
        if (type == "ivec4") {
            return VK_FORMAT_R32G32B32A32_SINT;
        }
        if (type == "uint") {
            return VK_FORMAT_R32_UINT;
        }
        if (type == "uvec2") {
            return VK_FORMAT_R32G32_UINT;
        }
        if (type == "uvec3") {
            return VK_FORMAT_R32G32B32_UINT;
        }
        if (type == "uvec4") {
            return VK_FORMAT_R32G32B32A32_UINT;
        }
        if (type == "bool") {
            return VK_FORMAT_R8_UINT; // GLSL bool maps to int in SPIR-V, typically 32-bit, but 8-bit here for compactness
        }
        if (type == "bvec2") {
            return VK_FORMAT_R8G8_UINT;
        }
        if (type == "bvec3") {
            return VK_FORMAT_R8G8B8_UINT;
        }
        if (type == "bvec4") {
            return VK_FORMAT_R8G8B8A8_UINT;
        }
        return VK_FORMAT_UNDEFINED;
    }
}
