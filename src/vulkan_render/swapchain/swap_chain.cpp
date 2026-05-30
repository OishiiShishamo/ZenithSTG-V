#include "vulkan_render/swapchain/swap_chain.h"

#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "vulkan_render/queue_family.h"
#include "window/window.h"

namespace zenithstgv {
void SwapChain::createSwapChain(SDL_Window *window, VkDevice &device,
                                VkPhysicalDevice &physical_device,
                                VkSurfaceKHR &surface,
                                VkSwapchainKHR &swap_chain,
                                std::vector<VkImage> &swap_chain_images,
                                VkFormat &swap_chain_image_format,
                                VkExtent2D &swap_chain_extent) {
	SwapChainSupportDetails swapChainSupport =
	    querySwapChainSupport(physical_device, surface);

	VkSurfaceFormatKHR surfaceFormat =
	    chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode =
	    chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(window, swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 &&
	    imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(physical_device, surface);
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
	                                 indices.presentFamily.value()};

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swap_chain) !=
	    VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, swap_chain, &imageCount, nullptr);
	swap_chain_images.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swap_chain, &imageCount,
	                        swap_chain_images.data());

	swap_chain_image_format = surfaceFormat.format;
	swap_chain_extent = extent;
}

SwapChainSupportDetails
SwapChain::querySwapChainSupport(VkPhysicalDevice device,
                                 VkSurfaceKHR surface) {
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
	                                          &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
	                                     nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
		                                     details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
	                                          &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
		    device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats) {
	for (const auto &availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
		    availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes) {
	for (const auto &availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D
SwapChain::chooseSwapExtent(SDL_Window *window,
                            const VkSurfaceCapabilitiesKHR &capabilities) {
	if (capabilities.currentExtent.width !=
	    std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	} else {
		int width = 0;
		int height = 0;
		SDL_GetWindowSizeInPixels(window, &width, &height);

		VkExtent2D actualExtent = {static_cast<uint32_t>(width),
		                           static_cast<uint32_t>(height)};

		actualExtent.width =
		    std::clamp(actualExtent.width, capabilities.minImageExtent.width,
		               capabilities.maxImageExtent.width);
		actualExtent.height =
		    std::clamp(actualExtent.height, capabilities.minImageExtent.height,
		               capabilities.maxImageExtent.height);

		return actualExtent;
	}
}
} // namespace zenithstgv
