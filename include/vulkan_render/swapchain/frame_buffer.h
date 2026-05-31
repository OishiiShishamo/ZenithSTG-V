#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_SWAPCHAIN_FRAME_BUFFER_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_SWAPCHAIN_FRAME_BUFFER_H_

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

namespace zenithstgv {
class FrameBuffer {
  public:
	static void
	createFramebuffers(VkDevice device, VkExtent2D swap_chain_extent,
	                   std::vector<VkImageView> &swap_chain_image_views,
	                   std::vector<VkFramebuffer> &swap_chain_framebuffers,
	                   VkRenderPass render_pass);
	static void cleanup(VkDevice device,
	                    std::vector<VkFramebuffer> &framebuffers);
};
} // namespace zenithstgv

#endif
