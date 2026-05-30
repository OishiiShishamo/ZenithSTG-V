#pragma once
#ifndef ZENITHSTGV_SRC_VULKAN_RENDER_SWAPCHAIN_SWAP_CHAIN_H_
#define ZENITHSTGV_SRC_VULKAN_RENDER_SWAPCHAIN_SWAP_CHAIN_H_

#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "vulkan_render/queue_family.h"
#include "window/window.h"

namespace zenithstgv {
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain {
  public:
	static void createSwapChain(SDL_Window *window, VkDevice &device,
	                            VkPhysicalDevice &physical_device,
	                            VkSurfaceKHR &surface,
	                            VkSwapchainKHR &swap_chain,
	                            std::vector<VkImage> &swap_chain_images,
	                            VkFormat &swap_chain_image_format,
	                            VkExtent2D &swap_chain_extent);
	static SwapChainSupportDetails
	querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

  private:
	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(
	    const std::vector<VkSurfaceFormatKHR> &availableFormats);

	static VkPresentModeKHR chooseSwapPresentMode(
	    const std::vector<VkPresentModeKHR> &availablePresentModes);

	static VkExtent2D
	chooseSwapExtent(SDL_Window *window,
	                 const VkSurfaceCapabilitiesKHR &capabilities);
};
} // namespace zenithstgv

#endif
