#pragma once
#ifndef ZENITHSTGV_SRC_VULKAN_RENDER_SWAPCHAIN_IMAGE_VIEW_H_
#define ZENITHSTGV_SRC_VULKAN_RENDER_SWAPCHAIN_IMAGE_VIEW_H_

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

namespace zenithstgv {
class ImageView {
  public:
	static void
	createImageViews(VkDevice device, std::vector<VkImage> &swap_chain_images,
	                 VkFormat &swap_chain_image_format,
	                 std::vector<VkImageView> &swap_chain_image_views);
};
} // namespace zenithstgv
#endif
