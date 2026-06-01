#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_SWAPCHAIN_FRAME_BUFFER_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_SWAPCHAIN_FRAME_BUFFER_H_

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
class FrameBuffer {
  public:
	static void createFramebuffers(
	    vk::Device device, vk::Extent2D swapChainExtent,
	    const std::vector<vk::UniqueImageView> &swapChainImageViews,
	    std::vector<vk::UniqueFramebuffer> &swapChainFramebuffers,
	    vk::RenderPass renderPass);
};
} // namespace zenithstgv

#endif
