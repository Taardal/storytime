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

    std::string get_vk_image_layout_name(VkImageLayout image_layout) {
        switch (image_layout) {
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
                return "";
        }
    }

    std::string get_vk_format_name(VkFormat format) {
        switch (format) {
            case VK_FORMAT_R4G4_UNORM_PACK8:
                return "VK_FORMAT_R4G4_UNORM_PACK8";
            case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
                return "VK_FORMAT_R4G4B4A4_UNORM_PACK16";
            case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
                return "VK_FORMAT_B4G4R4A4_UNORM_PACK16";
            case VK_FORMAT_R5G6B5_UNORM_PACK16:
                return "VK_FORMAT_R5G6B5_UNORM_PACK16";
            case VK_FORMAT_B5G6R5_UNORM_PACK16:
                return "VK_FORMAT_B5G6R5_UNORM_PACK16";
            case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
                return "VK_FORMAT_R5G5B5A1_UNORM_PACK16";
            case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
                return "VK_FORMAT_B5G5R5A1_UNORM_PACK16";
            case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
                return "VK_FORMAT_A1R5G5B5_UNORM_PACK16";
            case VK_FORMAT_R8_UNORM:
                return "VK_FORMAT_R8_UNORM";
            case VK_FORMAT_R8_SNORM:
                return "VK_FORMAT_R8_SNORM";
            case VK_FORMAT_R8_USCALED:
                return "VK_FORMAT_R8_USCALED";
            case VK_FORMAT_R8_SSCALED:
                return "VK_FORMAT_R8_SSCALED";
            case VK_FORMAT_R8_UINT:
                return "VK_FORMAT_R8_UINT";
            case VK_FORMAT_R8_SINT:
                return "VK_FORMAT_R8_SINT";
            case VK_FORMAT_R8_SRGB:
                return "VK_FORMAT_R8_SRGB";
            case VK_FORMAT_R8G8_UNORM:
                return "VK_FORMAT_R8G8_UNORM";
            case VK_FORMAT_R8G8_SNORM:
                return "VK_FORMAT_R8G8_SNORM";
            case VK_FORMAT_R8G8_USCALED:
                return "VK_FORMAT_R8G8_USCALED";
            case VK_FORMAT_R8G8_SSCALED:
                return "VK_FORMAT_R8G8_SSCALED";
            case VK_FORMAT_R8G8_UINT:
                return "VK_FORMAT_R8G8_UINT";
            case VK_FORMAT_R8G8_SINT:
                return "VK_FORMAT_R8G8_SINT";
            case VK_FORMAT_R8G8_SRGB:
                return "VK_FORMAT_R8G8_SRGB";
            case VK_FORMAT_R8G8B8_UNORM:
                return "VK_FORMAT_R8G8B8_UNORM";
            case VK_FORMAT_R8G8B8_SNORM:
                return "VK_FORMAT_R8G8B8_SNORM";
            case VK_FORMAT_R8G8B8_USCALED:
                return "VK_FORMAT_R8G8B8_USCALED";
            case VK_FORMAT_R8G8B8_SSCALED:
                return "VK_FORMAT_R8G8B8_SSCALED";
            case VK_FORMAT_R8G8B8_UINT:
                return "VK_FORMAT_R8G8B8_UINT";
            case VK_FORMAT_R8G8B8_SINT:
                return "VK_FORMAT_R8G8B8_SINT";
            case VK_FORMAT_R8G8B8_SRGB:
                return "VK_FORMAT_R8G8B8_SRGB";
            case VK_FORMAT_B8G8R8_UNORM:
                return "VK_FORMAT_B8G8R8_UNORM";
            case VK_FORMAT_B8G8R8_SNORM:
                return "VK_FORMAT_B8G8R8_SNORM";
            case VK_FORMAT_B8G8R8_USCALED:
                return "VK_FORMAT_B8G8R8_USCALED";
            case VK_FORMAT_B8G8R8_SSCALED:
                return "VK_FORMAT_B8G8R8_SSCALED";
            case VK_FORMAT_B8G8R8_UINT:
                return "VK_FORMAT_B8G8R8_UINT";
            case VK_FORMAT_B8G8R8_SINT:
                return "VK_FORMAT_B8G8R8_SINT";
            case VK_FORMAT_B8G8R8_SRGB:
                return "VK_FORMAT_B8G8R8_SRGB";
            case VK_FORMAT_R8G8B8A8_UNORM:
                return "VK_FORMAT_R8G8B8A8_UNORM";
            case VK_FORMAT_R8G8B8A8_SNORM:
                return "VK_FORMAT_R8G8B8A8_SNORM";
            case VK_FORMAT_R8G8B8A8_USCALED:
                return "VK_FORMAT_R8G8B8A8_USCALED";
            case VK_FORMAT_R8G8B8A8_SSCALED:
                return "VK_FORMAT_R8G8B8A8_SSCALED";
            case VK_FORMAT_R8G8B8A8_UINT:
                return "VK_FORMAT_R8G8B8A8_UINT";
            case VK_FORMAT_R8G8B8A8_SINT:
                return "VK_FORMAT_R8G8B8A8_SINT";
            case VK_FORMAT_R8G8B8A8_SRGB:
                return "VK_FORMAT_R8G8B8A8_SRGB";
            case VK_FORMAT_B8G8R8A8_UNORM:
                return "VK_FORMAT_B8G8R8A8_UNORM";
            case VK_FORMAT_B8G8R8A8_SNORM:
                return "VK_FORMAT_B8G8R8A8_SNORM";
            case VK_FORMAT_B8G8R8A8_USCALED:
                return "VK_FORMAT_B8G8R8A8_USCALED";
            case VK_FORMAT_B8G8R8A8_SSCALED:
                return "VK_FORMAT_B8G8R8A8_SSCALED";
            case VK_FORMAT_B8G8R8A8_UINT:
                return "VK_FORMAT_B8G8R8A8_UINT";
            case VK_FORMAT_B8G8R8A8_SINT:
                return "VK_FORMAT_B8G8R8A8_SINT";
            case VK_FORMAT_B8G8R8A8_SRGB:
                return "VK_FORMAT_B8G8R8A8_SRGB";
            case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
                return "VK_FORMAT_A8B8G8R8_UNORM_PACK32";
            case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
                return "VK_FORMAT_A8B8G8R8_SNORM_PACK32";
            case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
                return "VK_FORMAT_A8B8G8R8_USCALED_PACK32";
            case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
                return "VK_FORMAT_A8B8G8R8_SSCALED_PACK32";
            case VK_FORMAT_A8B8G8R8_UINT_PACK32:
                return "VK_FORMAT_A8B8G8R8_UINT_PACK32";
            case VK_FORMAT_A8B8G8R8_SINT_PACK32:
                return "VK_FORMAT_A8B8G8R8_SINT_PACK32";
            case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
                return "VK_FORMAT_A8B8G8R8_SRGB_PACK32";
            case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
                return "VK_FORMAT_A2R10G10B10_UNORM_PACK32";
            case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
                return "VK_FORMAT_A2R10G10B10_SNORM_PACK32";
            case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
                return "VK_FORMAT_A2R10G10B10_USCALED_PACK32";
            case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
                return "VK_FORMAT_A2R10G10B10_SSCALED_PACK32";
            case VK_FORMAT_A2R10G10B10_UINT_PACK32:
                return "VK_FORMAT_A2R10G10B10_UINT_PACK32";
            case VK_FORMAT_A2R10G10B10_SINT_PACK32:
                return "VK_FORMAT_A2R10G10B10_SINT_PACK32";
            case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
                return "VK_FORMAT_A2B10G10R10_UNORM_PACK32";
            case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
                return "VK_FORMAT_A2B10G10R10_SNORM_PACK32";
            case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
                return "VK_FORMAT_A2B10G10R10_USCALED_PACK32";
            case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
                return "VK_FORMAT_A2B10G10R10_SSCALED_PACK32";
            case VK_FORMAT_A2B10G10R10_UINT_PACK32:
                return "VK_FORMAT_A2B10G10R10_UINT_PACK32";
            case VK_FORMAT_A2B10G10R10_SINT_PACK32:
                return "VK_FORMAT_A2B10G10R10_SINT_PACK32";
            case VK_FORMAT_R16_UNORM:
                return "VK_FORMAT_R16_UNORM";
            case VK_FORMAT_R16_SNORM:
                return "VK_FORMAT_R16_SNORM";
            case VK_FORMAT_R16_USCALED:
                return "VK_FORMAT_R16_USCALED";
            case VK_FORMAT_R16_SSCALED:
                return "VK_FORMAT_R16_SSCALED";
            case VK_FORMAT_R16_UINT:
                return "VK_FORMAT_R16_UINT";
            case VK_FORMAT_R16_SINT:
                return "VK_FORMAT_R16_SINT";
            case VK_FORMAT_R16_SFLOAT:
                return "VK_FORMAT_R16_SFLOAT";
            case VK_FORMAT_R16G16_UNORM:
                return "VK_FORMAT_R16G16_UNORM";
            case VK_FORMAT_R16G16_SNORM:
                return "VK_FORMAT_R16G16_SNORM";
            case VK_FORMAT_R16G16_USCALED:
                return "VK_FORMAT_R16G16_USCALED";
            case VK_FORMAT_R16G16_SSCALED:
                return "VK_FORMAT_R16G16_SSCALED";
            case VK_FORMAT_R16G16_UINT:
                return "VK_FORMAT_R16G16_UINT";
            case VK_FORMAT_R16G16_SINT:
                return "VK_FORMAT_R16G16_SINT";
            case VK_FORMAT_R16G16_SFLOAT:
                return "VK_FORMAT_R16G16_SFLOAT";
            case VK_FORMAT_R16G16B16_UNORM:
                return "VK_FORMAT_R16G16B16_UNORM";
            case VK_FORMAT_R16G16B16_SNORM:
                return "VK_FORMAT_R16G16B16_SNORM";
            case VK_FORMAT_R16G16B16_USCALED:
                return "VK_FORMAT_R16G16B16_USCALED";
            case VK_FORMAT_R16G16B16_SSCALED:
                return "VK_FORMAT_R16G16B16_SSCALED";
            case VK_FORMAT_R16G16B16_UINT:
                return "VK_FORMAT_R16G16B16_UINT";
            case VK_FORMAT_R16G16B16_SINT:
                return "VK_FORMAT_R16G16B16_SINT";
            case VK_FORMAT_R16G16B16_SFLOAT:
                return "VK_FORMAT_R16G16B16_SFLOAT";
            case VK_FORMAT_R16G16B16A16_UNORM:
                return "VK_FORMAT_R16G16B16A16_UNORM";
            case VK_FORMAT_R16G16B16A16_SNORM:
                return "VK_FORMAT_R16G16B16A16_SNORM";
            case VK_FORMAT_R16G16B16A16_USCALED:
                return "VK_FORMAT_R16G16B16A16_USCALED";
            case VK_FORMAT_R16G16B16A16_SSCALED:
                return "VK_FORMAT_R16G16B16A16_SSCALED";
            case VK_FORMAT_R16G16B16A16_UINT:
                return "VK_FORMAT_R16G16B16A16_UINT";
            case VK_FORMAT_R16G16B16A16_SINT:
                return "VK_FORMAT_R16G16B16A16_SINT";
            case VK_FORMAT_R16G16B16A16_SFLOAT:
                return "VK_FORMAT_R16G16B16A16_SFLOAT";
            case VK_FORMAT_R32_UINT:
                return "VK_FORMAT_R32_UINT";
            case VK_FORMAT_R32_SINT:
                return "VK_FORMAT_R32_SINT";
            case VK_FORMAT_R32_SFLOAT:
                return "VK_FORMAT_R32_SFLOAT";
            case VK_FORMAT_R32G32_UINT:
                return "VK_FORMAT_R32G32_UINT";
            case VK_FORMAT_R32G32_SINT:
                return "VK_FORMAT_R32G32_SINT";
            case VK_FORMAT_R32G32_SFLOAT:
                return "VK_FORMAT_R32G32_SFLOAT";
            case VK_FORMAT_R32G32B32_UINT:
                return "VK_FORMAT_R32G32B32_UINT";
            case VK_FORMAT_R32G32B32_SINT:
                return "VK_FORMAT_R32G32B32_SINT";
            case VK_FORMAT_R32G32B32_SFLOAT:
                return "VK_FORMAT_R32G32B32_SFLOAT";
            case VK_FORMAT_R32G32B32A32_UINT:
                return "VK_FORMAT_R32G32B32A32_UINT";
            case VK_FORMAT_R32G32B32A32_SINT:
                return "VK_FORMAT_R32G32B32A32_SINT";
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return "VK_FORMAT_R32G32B32A32_SFLOAT";
            case VK_FORMAT_R64_UINT:
                return "VK_FORMAT_R64_UINT";
            case VK_FORMAT_R64_SINT:
                return "VK_FORMAT_R64_SINT";
            case VK_FORMAT_R64_SFLOAT:
                return "VK_FORMAT_R64_SFLOAT";
            case VK_FORMAT_R64G64_UINT:
                return "VK_FORMAT_R64G64_UINT";
            case VK_FORMAT_R64G64_SINT:
                return "VK_FORMAT_R64G64_SINT";
            case VK_FORMAT_R64G64_SFLOAT:
                return "VK_FORMAT_R64G64_SFLOAT";
            case VK_FORMAT_R64G64B64_UINT:
                return "VK_FORMAT_R64G64B64_UINT";
            case VK_FORMAT_R64G64B64_SINT:
                return "VK_FORMAT_R64G64B64_SINT";
            case VK_FORMAT_R64G64B64_SFLOAT:
                return "VK_FORMAT_R64G64B64_SFLOAT";
            case VK_FORMAT_R64G64B64A64_UINT:
                return "VK_FORMAT_R64G64B64A64_UINT";
            case VK_FORMAT_R64G64B64A64_SINT:
                return "VK_FORMAT_R64G64B64A64_SINT";
            case VK_FORMAT_R64G64B64A64_SFLOAT:
                return "VK_FORMAT_R64G64B64A64_SFLOAT";
            case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
                return "VK_FORMAT_B10G11R11_UFLOAT_PACK32";
            case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
                return "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32";
            case VK_FORMAT_D16_UNORM:
                return "VK_FORMAT_D16_UNORM";
            case VK_FORMAT_X8_D24_UNORM_PACK32:
                return "VK_FORMAT_X8_D24_UNORM_PACK32";
            case VK_FORMAT_D32_SFLOAT:
                return "VK_FORMAT_D32_SFLOAT";
            case VK_FORMAT_S8_UINT:
                return "VK_FORMAT_S8_UINT";
            case VK_FORMAT_D16_UNORM_S8_UINT:
                return "VK_FORMAT_D16_UNORM_S8_UINT";
            case VK_FORMAT_D24_UNORM_S8_UINT:
                return "VK_FORMAT_D24_UNORM_S8_UINT";
            case VK_FORMAT_D32_SFLOAT_S8_UINT:
                return "VK_FORMAT_D32_SFLOAT_S8_UINT";
            case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
                return "VK_FORMAT_BC1_RGB_UNORM_BLOCK";
            case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
                return "VK_FORMAT_BC1_RGB_SRGB_BLOCK";
            case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
                return "VK_FORMAT_BC1_RGBA_UNORM_BLOCK";
            case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
                return "VK_FORMAT_BC1_RGBA_SRGB_BLOCK";
            case VK_FORMAT_BC2_UNORM_BLOCK:
                return "VK_FORMAT_BC2_UNORM_BLOCK";
            case VK_FORMAT_BC2_SRGB_BLOCK:
                return "VK_FORMAT_BC2_SRGB_BLOCK";
            case VK_FORMAT_BC3_UNORM_BLOCK:
                return "VK_FORMAT_BC3_UNORM_BLOCK";
            case VK_FORMAT_BC3_SRGB_BLOCK:
                return "VK_FORMAT_BC3_SRGB_BLOCK";
            case VK_FORMAT_BC4_UNORM_BLOCK:
                return "VK_FORMAT_BC4_UNORM_BLOCK";
            case VK_FORMAT_BC4_SNORM_BLOCK:
                return "VK_FORMAT_BC4_SNORM_BLOCK";
            case VK_FORMAT_BC5_UNORM_BLOCK:
                return "VK_FORMAT_BC5_UNORM_BLOCK";
            case VK_FORMAT_BC5_SNORM_BLOCK:
                return "VK_FORMAT_BC5_SNORM_BLOCK";
            case VK_FORMAT_BC6H_UFLOAT_BLOCK:
                return "VK_FORMAT_BC6H_UFLOAT_BLOCK";
            case VK_FORMAT_BC6H_SFLOAT_BLOCK:
                return "VK_FORMAT_BC6H_SFLOAT_BLOCK";
            case VK_FORMAT_BC7_UNORM_BLOCK:
                return "VK_FORMAT_BC7_UNORM_BLOCK";
            case VK_FORMAT_BC7_SRGB_BLOCK:
                return "VK_FORMAT_BC7_SRGB_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK";
            case VK_FORMAT_EAC_R11_UNORM_BLOCK:
                return "VK_FORMAT_EAC_R11_UNORM_BLOCK";
            case VK_FORMAT_EAC_R11_SNORM_BLOCK:
                return "VK_FORMAT_EAC_R11_SNORM_BLOCK";
            case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
                return "VK_FORMAT_EAC_R11G11_UNORM_BLOCK";
            case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
                return "VK_FORMAT_EAC_R11G11_SNORM_BLOCK";
            case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_4x4_UNORM_BLOCK";
            case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_4x4_SRGB_BLOCK";
            case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_5x4_UNORM_BLOCK";
            case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_5x4_SRGB_BLOCK";
            case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_5x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_5x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_6x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_6x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_6x6_UNORM_BLOCK";
            case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_6x6_SRGB_BLOCK";
            case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_8x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_8x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_8x6_UNORM_BLOCK";
            case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_8x6_SRGB_BLOCK";
            case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_8x8_UNORM_BLOCK";
            case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_8x8_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_10x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_10x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_10x6_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_10x6_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_10x8_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_10x8_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_10x10_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_10x10_SRGB_BLOCK";
            case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_12x10_UNORM_BLOCK";
            case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_12x10_SRGB_BLOCK";
            case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_12x12_UNORM_BLOCK";
            case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_12x12_SRGB_BLOCK";
            case VK_FORMAT_G8B8G8R8_422_UNORM:
                return "VK_FORMAT_G8B8G8R8_422_UNORM";
            case VK_FORMAT_B8G8R8G8_422_UNORM:
                return "VK_FORMAT_B8G8R8G8_422_UNORM";
            case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM:
                return "VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM";
            case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM:
                return "VK_FORMAT_G8_B8R8_2PLANE_420_UNORM";
            case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM:
                return "VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM";
            case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM:
                return "VK_FORMAT_G8_B8R8_2PLANE_422_UNORM";
            case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM:
                return "VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM";
            case VK_FORMAT_R10X6_UNORM_PACK16:
                return "VK_FORMAT_R10X6_UNORM_PACK16";
            case VK_FORMAT_R10X6G10X6_UNORM_2PACK16:
                return "VK_FORMAT_R10X6G10X6_UNORM_2PACK16";
            case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16:
                return "VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16";
            case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
                return "VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16";
            case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
                return "VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_R12X4_UNORM_PACK16:
                return "VK_FORMAT_R12X4_UNORM_PACK16";
            case VK_FORMAT_R12X4G12X4_UNORM_2PACK16:
                return "VK_FORMAT_R12X4G12X4_UNORM_2PACK16";
            case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16:
                return "VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16";
            case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
                return "VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16";
            case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
                return "VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_G16B16G16R16_422_UNORM:
                return "VK_FORMAT_G16B16G16R16_422_UNORM";
            case VK_FORMAT_B16G16R16G16_422_UNORM:
                return "VK_FORMAT_B16G16R16G16_422_UNORM";
            case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM:
                return "VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM";
            case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM:
                return "VK_FORMAT_G16_B16R16_2PLANE_420_UNORM";
            case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM:
                return "VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM";
            case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM:
                return "VK_FORMAT_G16_B16R16_2PLANE_422_UNORM";
            case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM:
                return "VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM";
            case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM:
                return "VK_FORMAT_G8_B8R8_2PLANE_444_UNORM";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM:
                return "VK_FORMAT_G16_B16R16_2PLANE_444_UNORM";
            case VK_FORMAT_A4R4G4B4_UNORM_PACK16:
                return "VK_FORMAT_A4R4G4B4_UNORM_PACK16";
            case VK_FORMAT_A4B4G4R4_UNORM_PACK16:
                return "VK_FORMAT_A4B4G4R4_UNORM_PACK16";
            case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK";
            case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
                return "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
                return "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
                return "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
                return "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
                return "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
                return "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
                return "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
                return "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_R16G16_S10_5_NV:
                return "VK_FORMAT_R16G16_S10_5_NV";
            default:
                return "";
        }
    }

    std::string get_vk_image_tiling_name(VkImageTiling image_tiling) {
        switch (image_tiling) {
            case VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT:
                return "VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT";
            case VK_IMAGE_TILING_LINEAR:
                return "VK_IMAGE_TILING_LINEAR";
            case VK_IMAGE_TILING_OPTIMAL:
                return "VK_IMAGE_TILING_OPTIMAL";
            case VK_IMAGE_TILING_MAX_ENUM:
                return "VK_IMAGE_TILING_MAX_ENUM";
            default:
                return "";
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
