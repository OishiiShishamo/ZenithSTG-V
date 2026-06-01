#include "vulkan_render/swapchain/frame_buffer.h"

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
void FrameBuffer::createFramebuffers(
    vk::Device device, vk::Extent2D swapChainExtent,
    const std::vector<vk::UniqueImageView> &swapChainImageViews,
    std::vector<vk::UniqueFramebuffer> &swapChainFramebuffers,
    vk::RenderPass renderPass) {
	swapChainFramebuffers.clear();

	for (const auto &imageView : swapChainImageViews) {
		vk::ImageView attachments[] = {imageView.get()};

		vk::FramebufferCreateInfo framebufferInfo{{},
		                                          renderPass,
		                                          1,
		                                          attachments,
		                                          swapChainExtent.width,
		                                          swapChainExtent.height,
		                                          1};

		swapChainFramebuffers.push_back(
		    device.createFramebufferUnique(framebufferInfo));
	}
}
} // namespace zenithstgv
