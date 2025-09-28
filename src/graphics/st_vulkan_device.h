#pragma once

#include "st_vulkan_physical_device.h"

namespace Storytime {
    struct VulkanDeviceConfig {
        VulkanPhysicalDevice* physical_device = nullptr;
        std::string name = "VulkanDevice";
    };

    class VulkanDevice {
    public:
        typedef VulkanDeviceConfig Config;

    private:
        Config config;
        VkDevice device = nullptr;
        VkQueue graphics_queue = nullptr;
        VkQueue present_queue = nullptr;

    public:
        VulkanDevice(const Config& config);

        ~VulkanDevice();

        operator VkDevice() const;

        VkQueue get_graphics_queue() const;

        VkQueue get_present_queue() const;

        VkQueue get_queue(u32 queue_family_index, u32 queue_index = 0) const;

        void get_queue(u32 queue_family_index, u32 queue_index, VkQueue* queue) const;

        struct SubmitQueueConfig {
            VkQueue queue = nullptr;
            u32 submit_count = 0;
            VkSubmitInfo* submit_info = nullptr;
            VkFence fence = nullptr;
        };

        VkResult submit_queue(const SubmitQueueConfig& config) const;

        VkResult submit_queue(VkQueue queue, u32 submit_count, const VkSubmitInfo* submit_info, VkFence fence) const;

        VkResult create_swapchain(const VkSwapchainCreateInfoKHR& swapchain_create_info, VkSwapchainKHR* swapchain) const;

        void destroy_swapchain(VkSwapchainKHR swapchain) const;

        VkResult get_swapchain_images(VkSwapchainKHR swapchain, std::vector<VkImage>* images) const;

        VkResult get_swapchain_images(VkSwapchainKHR swapchain, u32* image_count, VkImage* images) const;

        struct AquireNextSwapchainImageConfig {
            VkSwapchainKHR swapchain;
            u32* image_index;
            VkSemaphore semaphore;
            VkFence fence;
            u64 timeout;
        };

        VkResult acquire_next_swapchain_image(const AquireNextSwapchainImageConfig& config) const;

        VkResult acquire_next_swapchain_image(VkSwapchainKHR swapchain, u64 timeout, VkSemaphore semaphore, VkFence fence, u32* image_index) const;

        VkResult create_image_view(const VkImageViewCreateInfo& image_view_create_info, VkImageView* image_view) const;

        void destroy_image_view(VkImageView image_view) const;

        VkResult create_render_pass(const VkRenderPassCreateInfo& render_pass_create_info, VkRenderPass* render_pass) const;

        void destroy_render_pass(VkRenderPass render_pass) const;

        VkResult create_framebuffer(const VkFramebufferCreateInfo& framebuffer_create_info, VkFramebuffer* framebuffer) const;

        void destroy_framebuffer(VkFramebuffer framebuffer) const;

        VkResult create_semaphore(const VkSemaphoreCreateInfo& semaphore_create_info, VkSemaphore* semaphore) const;

        void destroy_semaphore(VkSemaphore semaphore) const;

        VkResult create_fence(const VkFenceCreateInfo& fence_create_info, VkFence* fence) const;

        void destroy_fence(VkFence fence) const;

        struct WaitForFencesConfig {
            VkFence* fences = nullptr;
            u32 fence_count = 0;
            bool wait_for_all = true;
            u64 wait_timeout = UINT64_MAX;
        };

        VkResult wait_for_fences(const WaitForFencesConfig& config) const;

        VkResult wait_for_fences(u32 fence_count, const VkFence* fences, bool wait_for_all, u64 wait_timeout) const;

        struct ResetFencesConfig {
            u32 fence_count;
            VkFence* fences;
        };

        VkResult reset_fences(const ResetFencesConfig& config) const;

        VkResult reset_fences(u32 fence_count, const VkFence* fences) const;

        VkResult create_shader_module(const VkShaderModuleCreateInfo& shader_module_create_info, VkShaderModule* shader_module) const;

        void destroy_shader_module(VkShaderModule shader_module) const;

        VkResult create_graphics_pipeline(const VkGraphicsPipelineCreateInfo& graphics_pipeline_create_info, VkPipeline* graphics_pipeline) const;

        void destroy_graphics_pipeline(VkPipeline graphics_pipeline) const;

        VkResult create_pipeline_layout(const VkPipelineLayoutCreateInfo& pipeline_layout_create_info, VkPipelineLayout* pipeline_layout) const;

        void destroy_pipeline_layout(VkPipelineLayout pipeline_layout) const;

        VkResult create_command_pool(const VkCommandPoolCreateInfo& command_pool_create_info, VkCommandPool* command_pool) const;

        void destroy_command_pool(VkCommandPool command_pool) const;

        VkResult allocate_command_buffers(const VkCommandBufferAllocateInfo& command_buffer_allocate_info, VkCommandBuffer* command_buffer) const;

        VkResult set_object_name(void* object, VkObjectType object_type, const char* object_name) const;

        VkResult set_object_name(const VkDebugUtilsObjectNameInfoEXT& object_name_info) const;

        VkResult set_object_tag(const VkDebugUtilsObjectTagInfoEXT& object_tag_info) const;

        void begin_cmd_label(VkCommandBuffer command_buffer, const VkDebugUtilsLabelEXT& label) const;

        void insert_cmd_label(VkCommandBuffer command_buffer, const VkDebugUtilsLabelEXT& label) const;

        void end_cmd_label(VkCommandBuffer command_buffer) const;

        void begin_queue_label(VkQueue queue, const VkDebugUtilsLabelEXT& label) const;

        void insert_queue_label(VkQueue queue, const VkDebugUtilsLabelEXT& label) const;

        void end_queue_label(VkQueue queue) const;

        VkResult wait_until_idle() const;

    private:
        void create_device();

        void destroy_device() const;

        std::vector<const char*> get_enabled_extensions() const;

        const VkPhysicalDeviceFeatures& get_enabled_features() const;

        std::vector<VkDeviceQueueCreateInfo> get_queue_create_infos() const;
    };
}
