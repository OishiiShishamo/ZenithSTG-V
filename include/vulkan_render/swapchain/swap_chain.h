#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_SWAPCHAIN_SWAP_CHAIN_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_SWAPCHAIN_SWAP_CHAIN_H_

#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "vulkan_render/queue_family.h"
#include "window/window.h"

namespace zenithstgv {
struct SwapChainSupportDetails {
	vk::SurfaceCapabilitiesKHR capabilities;

	std::vector<vk::SurfaceFormatKHR> formats;

	std::vector<vk::PresentModeKHR> presentModes;
};

class SwapChain {
  public:
	static void createSwapChain(SDL_Window *window, vk::Device device,
	                            vk::PhysicalDevice physicalDevice,
	                            vk::SurfaceKHR surface,
	                            vk::UniqueSwapchainKHR &swapChain,
	                            std::vector<vk::Image> &swapChainImages,
	                            vk::Format &swapChainImageFormat,
	                            vk::Extent2D &swapChainExtent);

	static SwapChainSupportDetails
	querySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface);

  private:
	static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
	    const std::vector<vk::SurfaceFormatKHR> &availableFormats);

	static vk::PresentModeKHR chooseSwapPresentMode(
	    const std::vector<vk::PresentModeKHR> &availablePresentModes);

	static vk::Extent2D
	chooseSwapExtent(SDL_Window *window,
	                 const vk::SurfaceCapabilitiesKHR &capabilities);
};
} // namespace zenithstgv

#endif
