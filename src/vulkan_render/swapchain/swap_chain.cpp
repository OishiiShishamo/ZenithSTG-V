#include "vulkan_render/swapchain/swap_chain.h"

#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include "vulkan_render/queue_family.h"
#include "window/window.h"

namespace zenithstgv {
void SwapChain::createSwapChain(SDL_Window *window, vk::Device device,
                                vk::PhysicalDevice physicalDevice,
                                vk::SurfaceKHR surface,
                                vk::UniqueSwapchainKHR &swapChain,
                                std::vector<vk::Image> &swapChainImages,
                                vk::Format &swapChainImageFormat,
                                vk::Extent2D &swapChainExtent) {
	const auto swapChainSupport =
	    querySwapChainSupport(physicalDevice, surface);

	const auto surfaceFormat =
	    chooseSwapSurfaceFormat(swapChainSupport.formats);

	const auto presentMode =
	    chooseSwapPresentMode(swapChainSupport.presentModes);

	const auto extent = chooseSwapExtent(window, swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	if (swapChainSupport.capabilities.maxImageCount > 0 &&
	    imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	const auto indices = findQueueFamilies(physicalDevice, surface);

	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
	                                 indices.presentFamily.value()};

	vk::SwapchainCreateInfoKHR createInfo{
	    {},
	    surface,
	    imageCount,
	    surfaceFormat.format,
	    surfaceFormat.colorSpace,
	    extent,
	    1,
	    vk::ImageUsageFlagBits::eColorAttachment};

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;

		createInfo.queueFamilyIndexCount = 2;

		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

	createInfo.presentMode = presentMode;

	createInfo.clipped = VK_TRUE;

	swapChain = device.createSwapchainKHRUnique(createInfo);

	swapChainImages = device.getSwapchainImagesKHR(swapChain.get());

	swapChainImageFormat = surfaceFormat.format;

	swapChainExtent = extent;
}

SwapChainSupportDetails
SwapChain::querySwapChainSupport(vk::PhysicalDevice device,
                                 vk::SurfaceKHR surface) {
	SwapChainSupportDetails details;

	details.capabilities = device.getSurfaceCapabilitiesKHR(surface);

	details.formats = device.getSurfaceFormatsKHR(surface);

	details.presentModes = device.getSurfacePresentModesKHR(surface);

	return details;
}

vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR> &availableFormats) {
	for (const auto &availableFormat : availableFormats) {
		if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
		    availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

vk::PresentModeKHR SwapChain::chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR> &availablePresentModes) {
	for (const auto &availablePresentMode : availablePresentModes) {
		if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
			return availablePresentMode;
		}
	}

	return vk::PresentModeKHR::eFifo;
}

vk::Extent2D
SwapChain::chooseSwapExtent(SDL_Window *window,
                            const vk::SurfaceCapabilitiesKHR &capabilities) {
	if (capabilities.currentExtent.width !=
	    std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}

	int width = 0;
	int height = 0;

	SDL_GetWindowSizeInPixels(window, &width, &height);

	vk::Extent2D actualExtent{static_cast<uint32_t>(width),
	                          static_cast<uint32_t>(height)};

	actualExtent.width =
	    std::clamp(actualExtent.width, capabilities.minImageExtent.width,
	               capabilities.maxImageExtent.width);

	actualExtent.height =
	    std::clamp(actualExtent.height, capabilities.minImageExtent.height,
	               capabilities.maxImageExtent.height);

	return actualExtent;
}
} // namespace zenithstgv
