#include "st_vulkan_utils.h"

namespace Storytime {
    std::string get_vk_result_name(VkResult result) {
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

    std::string get_vk_result_description(VkResult result) {
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

    std::string get_vk_image_layout_name(VkImageLayout layout) {
        switch (layout) {
            case VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT:
                return "VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT";
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                return "VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL";
            case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
                return "VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL";
            case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
                return "VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL";
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                return "VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL";
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
                return "VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL";
            case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
                return "VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL";
            case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
                return "VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL";
            case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
                return "VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT";
            case VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR:
                return "VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR";
            case VK_IMAGE_LAYOUT_GENERAL:
                return "VK_IMAGE_LAYOUT_GENERAL";
            case VK_IMAGE_LAYOUT_MAX_ENUM:
                return "VK_IMAGE_LAYOUT_MAX_ENUM";
            case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
                return "VK_IMAGE_LAYOUT_PRESENT_SRC_KHR";
            case VK_IMAGE_LAYOUT_PREINITIALIZED:
                return "VK_IMAGE_LAYOUT_PREINITIALIZED";
            case VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL:
                return "VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL";
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                return "VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL";
            case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
                return "VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR";
            case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
                return "VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL";
            case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
                return "VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL";
            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
                return "VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL";
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
                return "VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL";
            case VK_IMAGE_LAYOUT_UNDEFINED:
                return "VK_IMAGE_LAYOUT_UNDEFINED";
            default:
                return "UNKNOWN_VK_IMAGE_LAYOUT";
        }
    }

    VkFormat get_vk_format(std::string_view glsl_type) {
        if (glsl_type == "float") {
            return VK_FORMAT_R32_SFLOAT;
        }
        if (glsl_type == "vec2") {
            return VK_FORMAT_R32G32_SFLOAT;
        }
        if (glsl_type == "vec3") {
            return VK_FORMAT_R32G32B32_SFLOAT;
        }
        if (glsl_type == "vec4") {
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        if (glsl_type == "double") {
            return VK_FORMAT_R64_SFLOAT;
        }
        if (glsl_type == "dvec2") {
            return VK_FORMAT_R64G64_SFLOAT;
        }
        if (glsl_type == "dvec3") {
            return VK_FORMAT_R64G64B64_SFLOAT;
        }
        if (glsl_type == "dvec4") {
            return VK_FORMAT_R64G64B64A64_SFLOAT;
        }
        if (glsl_type == "int") {
            return VK_FORMAT_R32_SINT;
        }
        if (glsl_type == "ivec2") {
            return VK_FORMAT_R32G32_SINT;
        }
        if (glsl_type == "ivec3") {
            return VK_FORMAT_R32G32B32_SINT;
        }
        if (glsl_type == "ivec4") {
            return VK_FORMAT_R32G32B32A32_SINT;
        }
        if (glsl_type == "uint") {
            return VK_FORMAT_R32_UINT;
        }
        if (glsl_type == "uvec2") {
            return VK_FORMAT_R32G32_UINT;
        }
        if (glsl_type == "uvec3") {
            return VK_FORMAT_R32G32B32_UINT;
        }
        if (glsl_type == "uvec4") {
            return VK_FORMAT_R32G32B32A32_UINT;
        }
        if (glsl_type == "bool") {
            return VK_FORMAT_R8_UINT; // GLSL bool maps to int in SPIR-V, typically 32-bit, but 8-bit here for compactness
        }
        if (glsl_type == "bvec2") {
            return VK_FORMAT_R8G8_UINT;
        }
        if (glsl_type == "bvec3") {
            return VK_FORMAT_R8G8B8_UINT;
        }
        if (glsl_type == "bvec4") {
            return VK_FORMAT_R8G8B8A8_UINT;
        }
        return VK_FORMAT_UNDEFINED;
    }
}
