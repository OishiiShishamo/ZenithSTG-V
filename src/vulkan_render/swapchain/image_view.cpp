#include "vulkan_render/swapchain/image_view.h"

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
void ImageView::createImageViews(
    vk::Device device, const std::vector<vk::Image> &swapChainImages,
    vk::Format swapChainImageFormat,
    std::vector<vk::UniqueImageView> &swapChainImageViews) {
	swapChainImageViews.clear();

	for (const auto &image : swapChainImages) {
		vk::ImageViewCreateInfo createInfo{
		    {},
		    image,
		    vk::ImageViewType::e2D,
		    swapChainImageFormat,
		    {vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity,
		     vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity},
		    {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

		swapChainImageViews.push_back(device.createImageViewUnique(createInfo));
	}
}
} // namespace zenithstgv
