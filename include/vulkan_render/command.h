#pragma once
#ifndef ZENITHSTGV_SRC_COMMAND_H_
#define ZENITHSTGV_SRC_COMMAND_H_

#include <stdexcept>

#include <vulkan/vulkan.h>

#include "vulkan_render/queue_family.h"

namespace zenithstgv {
class Command {
  public:
	static void createCommandPool(VkDevice device,
	                              VkPhysicalDevice physical_device,
	                              VkSurfaceKHR surface,
	                              VkCommandPool &command_pool);
	static void
	createCommandBuffer(VkDevice device, VkCommandPool command_pool,
	                    std::vector<VkCommandBuffer> &command_buffers);
	static void
	recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex,
	                    VkExtent2D swap_chain_extent,
	                    std::vector<VkFramebuffer> &swap_chain_framebuffers,
	                    VkRenderPass render_pass, VkPipeline graphics_pipeline);
};
} // namespace zenithstgv

#endif
