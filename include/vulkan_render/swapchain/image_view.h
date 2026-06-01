#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_SWAPCHAIN_IMAGE_VIEW_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_SWAPCHAIN_IMAGE_VIEW_H_

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
class ImageView {
  public:
	static void
	createImageViews(vk::Device device,
	                 const std::vector<vk::Image> &swapChainImages,
	                 vk::Format swapChainImageFormat,
	                 std::vector<vk::UniqueImageView> &swapChainImageViews);
};
} // namespace zenithstgv
#endif
