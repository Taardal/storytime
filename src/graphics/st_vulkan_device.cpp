#include "st_vulkan_device.h"

namespace Storytime {
    VulkanDevice::VulkanDevice(const Config& config) : config(config) {
        create_device();
    }

    VulkanDevice::~VulkanDevice() {
        destroy_device();
    }

    VulkanDevice::operator VkDevice() const {
        return device;
    }

    const VulkanPhysicalDevice& VulkanDevice::get_physical_device() const {
        return *config.physical_device;
    }

    u32 VulkanDevice::get_graphics_queue_family_index() const {
        return get_physical_device().get_graphics_queue_family_index();
    }

    u32 VulkanDevice::get_present_queue_family_index() const {
        return get_physical_device().get_present_queue_family_index();
    }

    VkQueue VulkanDevice::get_graphics_queue() const {
        return get_queue(get_graphics_queue_family_index());
    }

    VkQueue VulkanDevice::get_present_queue() const {
        return get_queue(get_present_queue_family_index());
    }

    VkQueue VulkanDevice::get_queue(u32 queue_family_index, u32 queue_index) const {
        VkQueue queue;
        get_queue(queue_family_index, queue_index, &queue);
        return queue;
    }

    void VulkanDevice::get_queue(u32 queue_family_index, u32 queue_index, VkQueue* queue) const {
        return vkGetDeviceQueue(device, queue_family_index, queue_index, queue);
    }

    VkResult VulkanDevice::submit_queue(const SubmitQueueConfig& config) const {
        return submit_queue(config.queue, config.submit_count, config.submit_info, config.fence);
    }

    VkResult VulkanDevice::submit_queue(VkQueue queue, u32 submit_count, const VkSubmitInfo* submit_info, VkFence fence) const {
        ST_ASSERT_NOT_NULL(queue);
        ST_ASSERT_NOT_NULL(submit_info);
        VkResult result = vkQueueSubmit(queue, submit_count, submit_info, fence);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not submit [{}] submit infos to queue: {}", submit_count, format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanDevice::create_swapchain(const VkSwapchainCreateInfoKHR& swapchain_create_info, VkSwapchainKHR* swapchain, std::string_view name) const {
        ST_ASSERT_NOT_NULL(swapchain);
        VkResult result = vkCreateSwapchainKHR(device, &swapchain_create_info, ST_VK_ALLOCATOR, swapchain);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create swapchain [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*swapchain, VK_OBJECT_TYPE_SWAPCHAIN_KHR, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set swapchain name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_swapchain(VkSwapchainKHR swapchain) const {
        vkDestroySwapchainKHR(device, swapchain, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::get_swapchain_images(VkSwapchainKHR swapchain, std::vector<VkImage>* images) const {
        ST_ASSERT_NOT_NULL(swapchain);
        ST_ASSERT_NOT_NULL(images);
        u32 image_count = 0;
        VkResult result = vkGetSwapchainImagesKHR(device, swapchain, &image_count, nullptr);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not get swapchain image count: {}", format_vk_result(result));
            return result;
        }
        images->resize(image_count);
        result = vkGetSwapchainImagesKHR(device, swapchain, &image_count, images->data());
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not get swapchain [{}] images: {}", images->size(), format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanDevice::get_swapchain_images(VkSwapchainKHR swapchain, u32* image_count, VkImage* images) const {
        ST_ASSERT_NOT_NULL(swapchain);
        ST_ASSERT_NOT_NULL(image_count);
        ST_ASSERT_NOT_NULL(images);
        VkResult result = vkGetSwapchainImagesKHR(device, swapchain, image_count, images);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not get swapchain images: {}", format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanDevice::acquire_next_swapchain_image(const AcquireNextSwapchainImage& config) const {
        return acquire_next_swapchain_image(config.swapchain, config.timeout, config.semaphore, config.fence, config.image_index);
    }

    VkResult VulkanDevice::acquire_next_swapchain_image(VkSwapchainKHR swapchain, u64 timeout, VkSemaphore semaphore, VkFence fence, u32* image_index) const {
        ST_ASSERT_NOT_NULL(swapchain);
        ST_ASSERT_NOT_NULL(image_index);
        VkResult result = vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, image_index);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not acquire next swapchain image: {}", format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanDevice::create_image(const VkImageCreateInfo& image_create_info, VkImage* image, std::string_view name) const {
        VkResult result = vkCreateImage(device, &image_create_info, ST_VK_ALLOCATOR, image);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create image [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*image, VK_OBJECT_TYPE_IMAGE, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set image name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_image(VkImage image) const {
        vkDestroyImage(device, image, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_image_view(const VkImageViewCreateInfo& image_view_create_info, VkImageView* image_view, std::string_view name) const {
        VkResult result = vkCreateImageView(device, &image_view_create_info, ST_VK_ALLOCATOR, image_view);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create image view [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*image_view, VK_OBJECT_TYPE_IMAGE_VIEW, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set image view name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_image_view(VkImageView image_view) const {
        vkDestroyImageView(device, image_view, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_render_pass(const VkRenderPassCreateInfo& render_pass_create_info, VkRenderPass* render_pass, std::string_view name) const {
        VkResult result = vkCreateRenderPass(device, &render_pass_create_info, ST_VK_ALLOCATOR, render_pass);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create render pass [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*render_pass, VK_OBJECT_TYPE_RENDER_PASS, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set render pass name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_render_pass(VkRenderPass render_pass) const {
        vkDestroyRenderPass(device, render_pass, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_framebuffer(const VkFramebufferCreateInfo& framebuffer_create_info, VkFramebuffer* framebuffer, std::string_view name) const {
        VkResult result = vkCreateFramebuffer(device, &framebuffer_create_info, ST_VK_ALLOCATOR, framebuffer);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create framebuffer [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*framebuffer, VK_OBJECT_TYPE_FRAMEBUFFER, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set framebuffer name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_framebuffer(VkFramebuffer framebuffer) const {
        vkDestroyFramebuffer(device, framebuffer, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_semaphore(const VkSemaphoreCreateInfo& semaphore_create_info, VkSemaphore* semaphore, std::string_view name) const {
        VkResult result = vkCreateSemaphore(device, &semaphore_create_info, ST_VK_ALLOCATOR, semaphore);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create semaphore [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*semaphore, VK_OBJECT_TYPE_SEMAPHORE, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set semaphore name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_semaphore(VkSemaphore semaphore) const {
        vkDestroySemaphore(device, semaphore, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_fence(const VkFenceCreateInfo& fence_create_info, VkFence* fence, std::string_view name) const {
        VkResult result = vkCreateFence(device, &fence_create_info, ST_VK_ALLOCATOR, fence);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create fence [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*fence, VK_OBJECT_TYPE_FENCE, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set fence name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_fence(VkFence fence) const {
        vkDestroyFence(device, fence, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::wait_for_fences(const WaitForFencesConfig& config) const {
        return wait_for_fences(config.fence_count, config.fences, config.wait_for_all, config.wait_timeout);
    }

    VkResult VulkanDevice::wait_for_fences(u32 fence_count, const VkFence* fences, bool wait_for_all, u64 wait_timeout) const {
        ST_ASSERT_GREATER_THAN_ZERO(fence_count);
        ST_ASSERT_NOT_NULL(fences);
        VkResult result = vkWaitForFences(device, fence_count, fences, wait_for_all, wait_timeout);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not wait for [{}] fences using timeout [{}] (wait for all [{}]): {}", fence_count, wait_timeout, wait_for_all, format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanDevice::wait_for_fence(VkFence fence, u64 wait_timeout) const {
        constexpr u32 fence_count = 1;
        constexpr bool wait_for_all = true;
        return wait_for_fences(fence_count, &fence, wait_for_all, wait_timeout);
    }

    VkResult VulkanDevice::reset_fences(const ResetFencesConfig& config) const {
        return reset_fences(config.fence_count, config.fences);
    }

    VkResult VulkanDevice::reset_fences(u32 fence_count, const VkFence* fences) const {
        ST_ASSERT_GREATER_THAN_ZERO(fence_count);
        ST_ASSERT_NOT_NULL(fences);
        VkResult result = vkResetFences(device, fence_count, fences);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not reset [{}] fences: {}", fence_count, format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanDevice::reset_fence(VkFence fence) const {
        constexpr u32 fence_count = 1;
        return reset_fences(fence_count, &fence);
    }

    VkResult VulkanDevice::create_shader_module(const VkShaderModuleCreateInfo& shader_module_create_info, VkShaderModule* shader_module, std::string_view name) const {
        VkResult result = vkCreateShaderModule(device, &shader_module_create_info, ST_VK_ALLOCATOR, shader_module);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not reset create shader module [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*shader_module, VK_OBJECT_TYPE_SHADER_MODULE, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set shader module name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_shader_module(VkShaderModule shader_module) const {
        vkDestroyShaderModule(device, shader_module, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_graphics_pipelines(u32 pipeline_count, const VkGraphicsPipelineCreateInfo* graphics_pipeline_create_infos, VkPipeline* graphics_pipelines) const {
        ST_ASSERT_GREATER_THAN_ZERO(pipeline_count);
        ST_ASSERT_NOT_NULL(graphics_pipeline_create_infos);
        constexpr VkPipelineCache pipeline_cache = nullptr;
        VkResult result = vkCreateGraphicsPipelines(device, pipeline_cache, pipeline_count, graphics_pipeline_create_infos, ST_VK_ALLOCATOR, graphics_pipelines);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create [{}] graphics pipelines: {}", pipeline_count, format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanDevice::create_graphics_pipeline(const VkGraphicsPipelineCreateInfo& graphics_pipeline_create_info, VkPipeline* graphics_pipeline, std::string_view name) const {
        constexpr u32 create_info_count = 1;
        VkResult result = create_graphics_pipelines(create_info_count, &graphics_pipeline_create_info, graphics_pipeline);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create graphics pipeline [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*graphics_pipeline, VK_OBJECT_TYPE_PIPELINE, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set graphics pipeline name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_graphics_pipeline(VkPipeline graphics_pipeline) const {
        vkDestroyPipeline(device, graphics_pipeline, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_pipeline_layout(const VkPipelineLayoutCreateInfo& pipeline_layout_create_info, VkPipelineLayout* pipeline_layout, std::string_view name) const {
        VkResult result = vkCreatePipelineLayout(device, &pipeline_layout_create_info, ST_VK_ALLOCATOR, pipeline_layout);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create pipeline layout [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*pipeline_layout, VK_OBJECT_TYPE_PIPELINE_LAYOUT, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set pipeline layout name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_pipeline_layout(VkPipelineLayout pipeline_layout) const {
        vkDestroyPipelineLayout(device, pipeline_layout, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_command_pool(const VkCommandPoolCreateInfo& command_pool_create_info, VkCommandPool* command_pool, std::string_view name) const {
        VkResult result = vkCreateCommandPool(device, &command_pool_create_info, ST_VK_ALLOCATOR, command_pool);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create command pool [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*command_pool, VK_OBJECT_TYPE_COMMAND_POOL, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set command pool name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_command_pool(VkCommandPool command_pool) const {
        vkDestroyCommandPool(device, command_pool, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::allocate_command_buffers(const VkCommandBufferAllocateInfo& command_buffer_allocate_info, VkCommandBuffer* command_buffers) const {
        VkResult result = vkAllocateCommandBuffers(device, &command_buffer_allocate_info, command_buffers);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not allocate [{}] command buffers: {}", command_buffer_allocate_info.commandBufferCount, format_vk_result(result));
        }
        return result;
    }

    void VulkanDevice::free_command_buffers(const FreeCommandBuffersConfig& config) const {
        free_command_buffers(config.command_pool, config.command_buffer_count, config.command_buffers);
    }

    void VulkanDevice::free_command_buffers(VkCommandPool command_pool, u32 command_buffer_count, const VkCommandBuffer* command_buffers) const {
        ST_ASSERT_NOT_NULL(command_pool);
        vkFreeCommandBuffers(device, command_pool, command_buffer_count, command_buffers);
    }

    void VulkanDevice::free_command_buffer(VkCommandPool command_pool, const VkCommandBuffer* command_buffer) const {
        constexpr u32 command_buffer_count = 1;
        vkFreeCommandBuffers(device, command_pool, command_buffer_count, command_buffer);
    }

    VkResult VulkanDevice::create_buffer(const VkBufferCreateInfo& buffer_create_info, VkBuffer* buffer, std::string_view name) const {
        VkResult result = vkCreateBuffer(device, &buffer_create_info, ST_VK_ALLOCATOR, buffer);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create buffer [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*buffer, VK_OBJECT_TYPE_BUFFER, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set buffer name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_buffer(VkBuffer buffer) const {
        vkDestroyBuffer(device, buffer, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::allocate_memory(const VkMemoryAllocateInfo& memory_allocate_info, VkDeviceMemory* memory, std::string_view name) const {
        VkResult result = vkAllocateMemory(device, &memory_allocate_info, ST_VK_ALLOCATOR, memory);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not allocate memory [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*memory, VK_OBJECT_TYPE_DEVICE_MEMORY, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set memory name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    VkResult VulkanDevice::bind_buffer_memory(const BindBufferMemoryConfig& config) const {
        return bind_buffer_memory(config.buffer, config.memory, config.memory_offset);
    }

    VkResult VulkanDevice::bind_buffer_memory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memory_offset) const {
        ST_ASSERT_NOT_NULL(buffer);
        ST_ASSERT_NOT_NULL(memory);
        VkResult result = vkBindBufferMemory(device, buffer, memory, memory_offset);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not bind buffer memory: {}", format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanDevice::bind_image_memory(const BindImageMemoryConfig& config) const {
        return bind_image_memory(config.image, config.memory, config.memory_offset);
    }

    VkResult VulkanDevice::bind_image_memory(VkImage image, VkDeviceMemory memory, VkDeviceSize memory_offset) const {
        ST_ASSERT_NOT_NULL(image);
        ST_ASSERT_NOT_NULL(memory);
        VkResult result = vkBindImageMemory(device, image, memory, memory_offset);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not bind image memory: {}", format_vk_result(result));
        }
        return result;
    }

    void VulkanDevice::get_buffer_memory_requirements(VkBuffer buffer, VkMemoryRequirements* memory_requirements) const {
        ST_ASSERT_NOT_NULL(buffer);
        vkGetBufferMemoryRequirements(device, buffer, memory_requirements);
    }

    VkMemoryRequirements VulkanDevice::get_buffer_memory_requirements(VkBuffer buffer) const {
        VkMemoryRequirements memory_requirements{};
        get_buffer_memory_requirements(buffer, &memory_requirements);
        return memory_requirements;
    }

    void VulkanDevice::get_image_memory_requirements(VkImage image, VkMemoryRequirements* memory_requirements) const {
        ST_ASSERT_NOT_NULL(image);
        vkGetImageMemoryRequirements(device, image, memory_requirements);
    }

    VkMemoryRequirements VulkanDevice::get_image_memory_requirements(VkImage image) const {
        VkMemoryRequirements memory_requirements{};
        get_image_memory_requirements(image, &memory_requirements);
        return memory_requirements;
    }

    void VulkanDevice::free_memory(VkDeviceMemory memory) const {
        vkFreeMemory(device, memory, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::map_memory(const MapMemoryConfig& config) const {
        return map_memory(config.memory, config.memory_offset, config.memory_size, config.flags, config.data);
    }

    VkResult VulkanDevice::map_memory(VkDeviceMemory memory, VkDeviceSize memory_offset, VkDeviceSize memory_size, VkMemoryMapFlags flags, void** data) const {
        VkResult result = vkMapMemory(device, memory, memory_offset, memory_size, flags, data);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not map memory: {}", format_vk_result(result));
        }
        return result;
    }

    void VulkanDevice::unmap_memory(VkDeviceMemory memory) const {
        return vkUnmapMemory(device, memory);
    }

    VkResult VulkanDevice::create_descriptor_set_layout(const VkDescriptorSetLayoutCreateInfo& descriptor_set_layout_create_info, VkDescriptorSetLayout* descriptor_set_layout, std::string_view name) const {
        VkResult result = vkCreateDescriptorSetLayout(device, &descriptor_set_layout_create_info, ST_VK_ALLOCATOR, descriptor_set_layout);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create descriptor set layout [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*descriptor_set_layout, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set descriptor set layout name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_descriptor_set_layout(VkDescriptorSetLayout descriptor_set_layout) const {
        vkDestroyDescriptorSetLayout(device, descriptor_set_layout, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_descriptor_pool(const VkDescriptorPoolCreateInfo& descriptor_pool_create_info, VkDescriptorPool* descriptor_pool, std::string_view name) const {
        ST_ASSERT_NOT_NULL(descriptor_pool);
        VkResult result = vkCreateDescriptorPool(device, &descriptor_pool_create_info, ST_VK_ALLOCATOR, descriptor_pool);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create descriptor pool [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*descriptor_pool, VK_OBJECT_TYPE_DESCRIPTOR_POOL, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set descriptor pool name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_descriptor_pool(VkDescriptorPool descriptor_pool) const {
        vkDestroyDescriptorPool(device, descriptor_pool, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::allocate_descriptor_sets(const VkDescriptorSetAllocateInfo& descriptor_set_allocate_info, VkDescriptorSet* descriptor_sets) const {
        VkResult result = vkAllocateDescriptorSets(device, &descriptor_set_allocate_info, descriptor_sets);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not allocate [{}] descriptor sets: {}", descriptor_set_allocate_info.descriptorSetCount, format_vk_result(result));
        }
        return result;
    }

    void VulkanDevice::free_descriptor_sets(const FreeDescriptorSetsConfig& config) const {
        free_descriptor_sets(config.descriptor_pool, config.descriptor_set_count, config.descriptor_sets);
    }

    void VulkanDevice::free_descriptor_sets(VkDescriptorPool descriptor_pool, u32 descriptor_set_count, const VkDescriptorSet* descriptor_sets) const {
        vkFreeDescriptorSets(device, descriptor_pool, descriptor_set_count, descriptor_sets);
    }

    void VulkanDevice::free_descriptor_set(VkDescriptorPool descriptor_pool, VkDescriptorSet descriptor_set) const {
        constexpr u32 descriptor_set_count = 1;
        free_descriptor_sets(descriptor_pool, descriptor_set_count, &descriptor_set);
    }

    void VulkanDevice::update_descriptor_sets(const UpdateDescriptorSetsConfig& config) const {
        update_descriptor_sets(config.descriptor_write_count, config.descriptor_writes, config.descriptor_copy_count, config.descriptor_copies);
    }

    void VulkanDevice::update_descriptor_sets(u32 descriptor_write_count, const VkWriteDescriptorSet* descriptor_writes, u32 descriptor_copy_count, const VkCopyDescriptorSet* descriptor_copies) const {
        vkUpdateDescriptorSets(device, descriptor_write_count, descriptor_writes, descriptor_copy_count, descriptor_copies);
    }

    VkResult VulkanDevice::create_sampler(const VkSamplerCreateInfo& sampler_create_info, VkSampler* sampler, std::string_view name) const {
        VkResult result = vkCreateSampler(device, &sampler_create_info, ST_VK_ALLOCATOR, sampler);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not create sampler [{}]: {}", name, format_vk_result(result));
            return result;
        }
        if (!name.empty()) {
            VkResult name_result = set_object_name(*sampler, VK_OBJECT_TYPE_SAMPLER, name.data());
            if (name_result != VK_SUCCESS) {
                ST_LOG_E("Could not set sampler name [{}]: {}", name, format_vk_result(name_result));
            }
        }
        return result;
    }

    void VulkanDevice::destroy_sampler(VkSampler sampler) const {
        vkDestroySampler(device, sampler, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::wait_until_idle() const {
        VkResult result = vkDeviceWaitIdle(device);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not wait for device to become idle: {}", format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanDevice::wait_until_queue_idle(VkQueue queue) const {
        VkResult result = vkQueueWaitIdle(queue);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not wait for queue to become idle: {}", format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanDevice::set_object_name(void* object, VkObjectType object_type, std::string_view object_name) const {
        return set_object_name(object, object_type, object_name.data());
    }

    VkResult VulkanDevice::set_object_name(void* object, VkObjectType object_type, const char* object_name) const {
        ST_ASSERT_NOT_NULL(object);
        ST_ASSERT_GREATER_THAN_ZERO(object_type);
        ST_ASSERT_NOT_NULL(object_name);
        ST_ASSERT_NOT_CEMPTY(object_name);

        VkDebugUtilsObjectNameInfoEXT object_name_info{};
        object_name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        object_name_info.objectType = object_type;
        object_name_info.objectHandle = (u64) object;
        object_name_info.pObjectName = object_name;

        VkResult result = set_object_name(object_name_info);
        if (result != VK_SUCCESS) {
            ST_LOG_E("Could not set name [{}] for object of type [{}]: {}", object_name, get_vk_object_type_name(object_type), format_vk_result(result));
        }
        return result;
    }

    VkResult VulkanDevice::set_object_name(const VkDebugUtilsObjectNameInfoEXT& object_name_info) const {
        auto set_object_name_fn = (PFN_vkSetDebugUtilsObjectNameEXT) vkGetDeviceProcAddr(device, "vkSetDebugUtilsObjectNameEXT");
        if (!set_object_name_fn) {
            ST_THROW("Could not get function to set Vulkan object name");
        }
        return set_object_name_fn(device, &object_name_info);
    }

    VkResult VulkanDevice::set_object_tag(const VkDebugUtilsObjectTagInfoEXT& object_tag_info) const {
        auto set_object_tag_fn = (PFN_vkSetDebugUtilsObjectTagEXT) vkGetDeviceProcAddr(device, "vkSetDebugUtilsObjectTagEXT");
        if (!set_object_tag_fn) {
            ST_THROW("Could not get function to set Vulkan object tag");
        }
        return set_object_tag_fn(device, &object_tag_info);
    }

    void VulkanDevice::begin_cmd_label(VkCommandBuffer command_buffer, const char* label_name, std::array<f32, 4> color) const {
        VkDebugUtilsLabelEXT label{};
        label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        label.pLabelName = label_name;
        std::copy_n(color.data(), 4, label.color);
        begin_cmd_label(command_buffer, label);
    }

    void VulkanDevice::begin_cmd_label(VkCommandBuffer command_buffer, const VkDebugUtilsLabelEXT& label) const {
        auto begin_cmd_label_fn = (PFN_vkCmdBeginDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkCmdBeginDebugUtilsLabelEXT");
        if (!begin_cmd_label_fn) {
            ST_THROW("Could not get function to begin cmd label [" << label.pLabelName << "]");
        }
        begin_cmd_label_fn(command_buffer, &label);
    }

    void VulkanDevice::insert_cmd_label(VkCommandBuffer command_buffer, const char* label_name, std::array<f32, 4> color) const {
        VkDebugUtilsLabelEXT label{};
        label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        label.pLabelName = label_name;
        std::copy_n(color.data(), 4, label.color);
        insert_cmd_label(command_buffer, label);
    }

    void VulkanDevice::insert_cmd_label(VkCommandBuffer command_buffer, const VkDebugUtilsLabelEXT& label) const {
        auto insert_cmd_label_fn = (PFN_vkCmdInsertDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkCmdInsertDebugUtilsLabelEXT");
        if (!insert_cmd_label_fn) {
            ST_THROW("Could not get function to insert cmd label [" << label.pLabelName << "]");
        }
        insert_cmd_label_fn(command_buffer, &label);
    }

    void VulkanDevice::end_cmd_label(VkCommandBuffer command_buffer) const {
        auto end_cmd_label_fn = (PFN_vkCmdEndDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkCmdEndDebugUtilsLabelEXT");
        if (!end_cmd_label_fn) {
            ST_THROW("Could not get function to end cmd debug label");
        }
        end_cmd_label_fn(command_buffer);
    }

    void VulkanDevice::begin_queue_label(VkQueue queue, const char* label_name, std::array<f32, 4> color) const {
        VkDebugUtilsLabelEXT label{};
        label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        label.pLabelName = label_name;
        std::copy_n(color.data(), 4, label.color);
        begin_queue_label(queue, label);
    }

    void VulkanDevice::begin_queue_label(VkQueue queue, const VkDebugUtilsLabelEXT& label) const {
        auto begin_queue_label_fn = (PFN_vkQueueBeginDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkQueueBeginDebugUtilsLabelEXT");
        if (!begin_queue_label_fn) {
            ST_THROW("Could not get function to begin queue label [" << label.pLabelName << "]");
        }
        begin_queue_label_fn(queue, &label);
    }

    void VulkanDevice::insert_queue_label(VkQueue queue, const char* label_name, std::array<f32, 4> color) const {
        VkDebugUtilsLabelEXT label{};
        label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        label.pLabelName = label_name;
        std::copy_n(color.data(), 4, label.color);
        insert_queue_label(queue, label);
    }

    void VulkanDevice::insert_queue_label(VkQueue queue, const VkDebugUtilsLabelEXT& label) const {
        auto insert_queue_label_fn = (PFN_vkQueueInsertDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkQueueInsertDebugUtilsLabelEXT");
        if (!insert_queue_label_fn) {
            ST_THROW("Could not get function to insert queue label [" << label.pLabelName << "]");
        }
        insert_queue_label_fn(queue, &label);
    }

    void VulkanDevice::end_queue_label(VkQueue queue) const {
        auto end_queue_label_fn = (PFN_vkQueueEndDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkQueueEndDebugUtilsLabelEXT");
        if (!end_queue_label_fn) {
            ST_THROW("Could not get function to end queue label");
        }
        end_queue_label_fn(queue);
    }

    void VulkanDevice::create_device() {
        const VulkanPhysicalDevice& physical_device = *config.physical_device;

        const VkPhysicalDeviceFeatures& enabled_features = physical_device.get_features();
        const std::vector<const char*>& enabled_extensions = physical_device.enabled_extensions;
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos = get_queue_create_infos();

        VkDeviceCreateInfo device_create_info{};
        device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.pEnabledFeatures = &enabled_features;
        device_create_info.enabledExtensionCount = (u32) enabled_extensions.size();
        device_create_info.ppEnabledExtensionNames = enabled_extensions.data();
        device_create_info.pQueueCreateInfos = queue_create_infos.data();
        device_create_info.queueCreateInfoCount = (u32) queue_create_infos.size();

        VkResult result = physical_device.create_device(device_create_info, &device);
        if (result != VK_SUCCESS) {
            ST_THROW("Could not create device [" << config.name << "]: " << format_vk_result(result));
        }

        VkResult device_name_result = set_object_name(device, VK_OBJECT_TYPE_DEVICE, config.name.c_str());
        if (device_name_result != VK_SUCCESS) {
            ST_LOG_E("Could not set device name [{}]: {}", config.name, format_vk_result(device_name_result));
        }

        std::string graphics_queue_name = std::format("{} graphics queue", config.name);
        VkResult graphics_queue_name_result = set_object_name(get_graphics_queue(), VK_OBJECT_TYPE_QUEUE, graphics_queue_name.c_str());
        if (graphics_queue_name_result != VK_SUCCESS) {
            ST_LOG_E("Could not set graphics queue name [{}]: {}", graphics_queue_name, format_vk_result(graphics_queue_name_result));
        }

        std::string present_queue_name = std::format("{} present queue", config.name);
        VkResult present_queue_name_result = set_object_name(get_present_queue(), VK_OBJECT_TYPE_QUEUE, present_queue_name.c_str());
        if (present_queue_name_result != VK_SUCCESS) {
            ST_LOG_E("Could not set present queue name [{}]: {}", present_queue_name, format_vk_result(present_queue_name_result));
        }
    }

    void VulkanDevice::destroy_device() const {
        vkDestroyDevice(device, ST_VK_ALLOCATOR);
    }

    std::vector<VkDeviceQueueCreateInfo> VulkanDevice::get_queue_create_infos() const {
        const VulkanPhysicalDevice& physical_device = *config.physical_device;

        std::set<u32> queue_families = {
            physical_device.get_graphics_queue_family_index(),
            physical_device.get_present_queue_family_index(),
        };

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        queue_create_infos.reserve(queue_families.size());

        // Vulkan lets you assign priorities to queues to influence the scheduling of command buffer execution using
        // floating point numbers between 0.0 and 1.0. This is required even if there is only a single queue.
        static constexpr f32 queue_priority = 1.0f;

        for (u32 queue_family : queue_families) {
            VkDeviceQueueCreateInfo queue_create_info{};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queue_family;
            queue_create_info.queueCount = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_create_info);
        }

        return queue_create_infos;
    }
}
