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
        vkGetDeviceQueue(device, queue_family_index, queue_index, &queue);
        return queue;
    }

    void VulkanDevice::get_queue(u32 queue_family_index, u32 queue_index, VkQueue* queue) const {
        return vkGetDeviceQueue(device, queue_family_index, queue_index, queue);
    }

    VkResult VulkanDevice::submit_queue(const SubmitQueue& submit_queue) const {
        return vkQueueSubmit(submit_queue.queue, submit_queue.submit_count, submit_queue.submit_info, submit_queue.fence);
    }

    VkResult VulkanDevice::submit_queue(VkQueue queue, u32 submit_count, const VkSubmitInfo* submit_info, VkFence fence) const {
        return vkQueueSubmit(queue, submit_count, submit_info, fence);
    }

    VkResult VulkanDevice::create_swapchain(const VkSwapchainCreateInfoKHR& swapchain_create_info, VkSwapchainKHR* swapchain) const {
        return vkCreateSwapchainKHR(device, &swapchain_create_info, ST_VK_ALLOCATOR, swapchain);
    }

    void VulkanDevice::destroy_swapchain(VkSwapchainKHR swapchain) const {
        vkDestroySwapchainKHR(device, swapchain, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::get_swapchain_images(VkSwapchainKHR swapchain, std::vector<VkImage>* images) const {
        u32 image_count = 0;
        VkResult result = get_swapchain_images(swapchain, &image_count, nullptr);
        if (result != VK_SUCCESS) {
            return result;
        }
        images->resize(image_count);
        return get_swapchain_images(swapchain, &image_count, images->data());
    }

    VkResult VulkanDevice::get_swapchain_images(VkSwapchainKHR swapchain, u32* image_count, VkImage* images) const {
        return vkGetSwapchainImagesKHR(device, swapchain, image_count, images);
    }

    VkResult VulkanDevice::acquire_next_swapchain_image(const AcquireNextSwapchainImage& acquire_next_swapchain_image) const {
        return vkAcquireNextImageKHR(device, acquire_next_swapchain_image.swapchain, acquire_next_swapchain_image.timeout, acquire_next_swapchain_image.semaphore, acquire_next_swapchain_image.fence, acquire_next_swapchain_image.image_index);
    }

    VkResult VulkanDevice::acquire_next_swapchain_image(VkSwapchainKHR swapchain, u64 timeout, VkSemaphore semaphore, VkFence fence, u32* image_index) const {
        return vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, image_index);
    }

    VkResult VulkanDevice::create_image(const VkImageCreateInfo& image_create_info, VkImage* image) const {
        return vkCreateImage(device, &image_create_info, ST_VK_ALLOCATOR, image);
    }

    void VulkanDevice::destroy_image(VkImage image) const {
        vkDestroyImage(device, image, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_image_view(const VkImageViewCreateInfo& image_view_create_info, VkImageView* image_view) const {
        return vkCreateImageView(device, &image_view_create_info, ST_VK_ALLOCATOR, image_view);
    }

    void VulkanDevice::destroy_image_view(VkImageView image_view) const {
        vkDestroyImageView(device, image_view, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_render_pass(const VkRenderPassCreateInfo& render_pass_create_info, VkRenderPass* render_pass) const {
        return vkCreateRenderPass(device, &render_pass_create_info, ST_VK_ALLOCATOR, render_pass);
    }

    void VulkanDevice::destroy_render_pass(VkRenderPass render_pass) const {
        vkDestroyRenderPass(device, render_pass, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_framebuffer(const VkFramebufferCreateInfo& framebuffer_create_info, VkFramebuffer* framebuffer) const {
        return vkCreateFramebuffer(device, &framebuffer_create_info, ST_VK_ALLOCATOR, framebuffer);
    }

    void VulkanDevice::destroy_framebuffer(VkFramebuffer framebuffer) const {
        vkDestroyFramebuffer(device, framebuffer, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_semaphore(const VkSemaphoreCreateInfo& semaphore_create_info, VkSemaphore* semaphore) const {
        return vkCreateSemaphore(device, &semaphore_create_info, ST_VK_ALLOCATOR, semaphore);
    }

    void VulkanDevice::destroy_semaphore(VkSemaphore semaphore) const {
        vkDestroySemaphore(device, semaphore, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_fence(const VkFenceCreateInfo& fence_create_info, VkFence* fence) const {
        return vkCreateFence(device, &fence_create_info, ST_VK_ALLOCATOR, fence);
    }

    void VulkanDevice::destroy_fence(VkFence fence) const {
        vkDestroyFence(device, fence, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::wait_for_fences(const WaitForFences& wait_for_fences) const {
        return vkWaitForFences(device, wait_for_fences.fence_count, wait_for_fences.fences, wait_for_fences.wait_for_all, wait_for_fences.wait_timeout);
    }

    VkResult VulkanDevice::wait_for_fences(u32 fence_count, const VkFence* fences, bool wait_for_all, u64 wait_timeout) const {
        return vkWaitForFences(device, fence_count, fences, wait_for_all, wait_timeout);
    }

    VkResult VulkanDevice::wait_for_fence(VkFence fence, u64 wait_timeout) const {
        constexpr u32 fence_count = 1;
        constexpr bool wait_for_all = true;
        return vkWaitForFences(device, fence_count, &fence, wait_for_all, wait_timeout);
    }

    VkResult VulkanDevice::reset_fences(const ResetFences& reset_fences) const {
        return vkResetFences(device, reset_fences.fence_count, reset_fences.fences);
    }

    VkResult VulkanDevice::reset_fences(u32 fence_count, const VkFence* fences) const {
        return vkResetFences(device, fence_count, fences);
    }

    VkResult VulkanDevice::reset_fence(VkFence fence) const {
        constexpr u32 fence_count = 1;
        return vkResetFences(device, fence_count, &fence);
    }

    VkResult VulkanDevice::create_shader_module(const VkShaderModuleCreateInfo& shader_module_create_info, VkShaderModule* shader_module) const {
        return vkCreateShaderModule(device, &shader_module_create_info, ST_VK_ALLOCATOR, shader_module);
    }

    void VulkanDevice::destroy_shader_module(VkShaderModule shader_module) const {
        vkDestroyShaderModule(device, shader_module, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_graphics_pipeline(const VkGraphicsPipelineCreateInfo& graphics_pipeline_create_info, VkPipeline* graphics_pipeline) const {
        constexpr u32 create_info_count = 1;
        constexpr VkPipelineCache pipeline_cache = nullptr;
        return vkCreateGraphicsPipelines(device, pipeline_cache, create_info_count, &graphics_pipeline_create_info, ST_VK_ALLOCATOR, graphics_pipeline);
    }

    void VulkanDevice::destroy_graphics_pipeline(VkPipeline graphics_pipeline) const {
        vkDestroyPipeline(device, graphics_pipeline, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_pipeline_layout(const VkPipelineLayoutCreateInfo& pipeline_layout_create_info, VkPipelineLayout* pipeline_layout) const {
        return vkCreatePipelineLayout(device, &pipeline_layout_create_info, ST_VK_ALLOCATOR, pipeline_layout);
    }

    void VulkanDevice::destroy_pipeline_layout(VkPipelineLayout pipeline_layout) const {
        vkDestroyPipelineLayout(device, pipeline_layout, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_command_pool(const VkCommandPoolCreateInfo& command_pool_create_info, VkCommandPool* command_pool) const {
        return vkCreateCommandPool(device, &command_pool_create_info, ST_VK_ALLOCATOR, command_pool);
    }

    void VulkanDevice::destroy_command_pool(VkCommandPool command_pool) const {
        vkDestroyCommandPool(device, command_pool, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::allocate_command_buffers(const VkCommandBufferAllocateInfo& command_buffer_allocate_info, VkCommandBuffer* command_buffers) const {
        return vkAllocateCommandBuffers(device, &command_buffer_allocate_info, command_buffers);
    }

    void VulkanDevice::free_command_buffers(const FreeCommandBuffers& free_command_buffers) const {
        vkFreeCommandBuffers(device, free_command_buffers.command_pool, free_command_buffers.command_buffer_count, free_command_buffers.command_buffers);
    }

    void VulkanDevice::free_command_buffers(VkCommandPool command_pool, u32 command_buffer_count, const VkCommandBuffer* command_buffers) const {
        vkFreeCommandBuffers(device, command_pool, command_buffer_count, command_buffers);
    }

    void VulkanDevice::free_command_buffer(VkCommandPool command_pool, VkCommandBuffer command_buffer) const {
        constexpr u32 command_buffer_count = 1;
        vkFreeCommandBuffers(device, command_pool, command_buffer_count, &command_buffer);
    }

    VkResult VulkanDevice::create_buffer(const VkBufferCreateInfo& buffer_create_info, VkBuffer* buffer) const {
        return vkCreateBuffer(device, &buffer_create_info, ST_VK_ALLOCATOR, buffer);
    }

    void VulkanDevice::destroy_buffer(VkBuffer buffer) const {
        vkDestroyBuffer(device, buffer, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::allocate_memory(const VkMemoryAllocateInfo& memory_allocate_info, VkDeviceMemory* memory) const {
        return vkAllocateMemory(device, &memory_allocate_info, ST_VK_ALLOCATOR, memory);
    }

    VkResult VulkanDevice::bind_buffer_memory(const BindBufferMemory& bind_buffer_memory) const {
        return vkBindBufferMemory(device, bind_buffer_memory.buffer, bind_buffer_memory.memory, bind_buffer_memory.memory_offset);
    }

    VkResult VulkanDevice::bind_buffer_memory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memory_offset) const {
        return vkBindBufferMemory(device, buffer, memory, memory_offset);
    }

    VkResult VulkanDevice::bind_image_memory(const BindImageMemory& bind_image_memory) const {
        return vkBindImageMemory(device, bind_image_memory.image, bind_image_memory.memory, bind_image_memory.memory_offset);
    }

    VkResult VulkanDevice::bind_image_memory(VkImage image, VkDeviceMemory memory, VkDeviceSize memory_offset) const {
        return vkBindImageMemory(device, image, memory, memory_offset);
    }

    void VulkanDevice::get_buffer_memory_requirements(VkBuffer buffer, VkMemoryRequirements* memory_requirements) const {
        vkGetBufferMemoryRequirements(device, buffer, memory_requirements);
    }

    VkMemoryRequirements VulkanDevice::get_buffer_memory_requirements(VkBuffer buffer) const {
        VkMemoryRequirements memory_requirements{};
        vkGetBufferMemoryRequirements(device, buffer, &memory_requirements);
        return memory_requirements;
    }

    void VulkanDevice::get_image_memory_requirements(VkImage image, VkMemoryRequirements* memory_requirements) const {
        vkGetImageMemoryRequirements(device, image, memory_requirements);
    }

    VkMemoryRequirements VulkanDevice::get_image_memory_requirements(VkImage image) const {
        VkMemoryRequirements memory_requirements{};
        vkGetImageMemoryRequirements(device, image, &memory_requirements);
        return memory_requirements;
    }

    void VulkanDevice::free_memory(VkDeviceMemory memory) const {
        vkFreeMemory(device, memory, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::map_memory(const MapMemoryConfig& config) const {
        return vkMapMemory(device, config.memory, config.memory_offset, config.memory_size, config.flags, config.data);
    }

    VkResult VulkanDevice::map_memory(VkDeviceMemory memory, VkDeviceSize memory_offset, VkDeviceSize memory_size, VkMemoryMapFlags flags, void** data) const {
        return vkMapMemory(device, memory, memory_offset, memory_size, flags, data);
    }

    void VulkanDevice::unmap_memory(VkDeviceMemory memory) const {
        return vkUnmapMemory(device, memory);
    }

    VkResult VulkanDevice::create_descriptor_set_layout(const VkDescriptorSetLayoutCreateInfo& descriptor_set_layout_create_info, VkDescriptorSetLayout* descriptor_set_layout) const {
        return vkCreateDescriptorSetLayout(device, &descriptor_set_layout_create_info, ST_VK_ALLOCATOR, descriptor_set_layout);
    }

    void VulkanDevice::destroy_descriptor_set_layout(VkDescriptorSetLayout descriptor_set_layout) const {
        vkDestroyDescriptorSetLayout(device, descriptor_set_layout, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::create_descriptor_pool(const VkDescriptorPoolCreateInfo& descriptor_pool_create_info, VkDescriptorPool* descriptor_pool) const {
        return vkCreateDescriptorPool(device, &descriptor_pool_create_info, ST_VK_ALLOCATOR, descriptor_pool);
    }

    void VulkanDevice::destroy_descriptor_pool(VkDescriptorPool descriptor_pool) const {
        vkDestroyDescriptorPool(device, descriptor_pool, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::allocate_descriptor_sets(const VkDescriptorSetAllocateInfo& descriptor_set_allocate_info, VkDescriptorSet* descriptor_sets) const {
        return vkAllocateDescriptorSets(device, &descriptor_set_allocate_info, descriptor_sets);
    }

    void VulkanDevice::free_descriptor_sets(const FreeDescriptorSets& free_descriptor_sets) const {
        vkFreeDescriptorSets(device, free_descriptor_sets.descriptor_pool, free_descriptor_sets.descriptor_set_count, free_descriptor_sets.descriptor_sets);
    }

    void VulkanDevice::free_descriptor_sets(VkDescriptorPool descriptor_pool, u32 descriptor_set_count, const VkDescriptorSet* descriptor_sets) const {
        vkFreeDescriptorSets(device, descriptor_pool, descriptor_set_count, descriptor_sets);
    }

    void VulkanDevice::free_descriptor_set(VkDescriptorPool descriptor_pool, VkDescriptorSet descriptor_set) const {
        constexpr u32 descriptor_set_count = 1;
        vkFreeDescriptorSets(device, descriptor_pool, descriptor_set_count, &descriptor_set);
    }

    void VulkanDevice::update_descriptor_sets(const UpdateDescriptorSets& update_descriptor_sets) const {
        vkUpdateDescriptorSets(device, update_descriptor_sets.descriptor_write_count, update_descriptor_sets.descriptor_writes, update_descriptor_sets.descriptor_copy_count, update_descriptor_sets.descriptor_copies);
    }

    void VulkanDevice::update_descriptor_sets(u32 descriptor_write_count, const VkWriteDescriptorSet* descriptor_writes, u32 descriptor_copy_count, const VkCopyDescriptorSet* descriptor_copies) const {
        vkUpdateDescriptorSets(device, descriptor_write_count, descriptor_writes, descriptor_copy_count, descriptor_copies);
    }

    VkResult VulkanDevice::create_sampler(const VkSamplerCreateInfo& sampler_create_info, VkSampler* sampler) const {
        return vkCreateSampler(device, &sampler_create_info, ST_VK_ALLOCATOR, sampler);
    }

    void VulkanDevice::destroy_sampler(VkSampler sampler) const {
        vkDestroySampler(device, sampler, ST_VK_ALLOCATOR);
    }

    VkResult VulkanDevice::wait_until_idle() const {
        return vkDeviceWaitIdle(device);
    }

    VkResult VulkanDevice::wait_until_queue_idle(VkQueue queue) const {
        return vkQueueWaitIdle(queue);
    }

    VkResult VulkanDevice::set_object_name(void* object, VkObjectType object_type, const char* object_name) const {
        VkDebugUtilsObjectNameInfoEXT object_name_info{};
        object_name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        object_name_info.objectType = object_type;
        object_name_info.objectHandle = (uint64_t) object;
        object_name_info.pObjectName = object_name;
        return set_object_name(object_name_info);
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

        if (physical_device.create_device(device_create_info, &device) != VK_SUCCESS) {
            ST_THROW("Could not create Vulkan device");
        }

        if (set_object_name(device, VK_OBJECT_TYPE_DEVICE, config.name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set Vulkan device name [" << config.name << "]");
        }

        std::string graphics_queue_name = std::format("{} graphics queue", config.name);
        if (set_object_name(get_graphics_queue(), VK_OBJECT_TYPE_QUEUE, graphics_queue_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set Vulkan device graphics queue name [" << graphics_queue_name << "]");
        }

        std::string present_queue_name = std::format("{} present queue", config.name);
        if (set_object_name(get_present_queue(), VK_OBJECT_TYPE_QUEUE, present_queue_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set Vulkan device present queue name [" << present_queue_name << "]");
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
