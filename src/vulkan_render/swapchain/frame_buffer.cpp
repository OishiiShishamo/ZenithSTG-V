#include "vulkan_render/swapchain/frame_buffer.h"

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

namespace zenithstgv {
void FrameBuffer::createFramebuffers(
    VkDevice device, VkExtent2D swap_chain_extent,
    std::vector<VkImageView> &swap_chain_image_views,
    std::vector<VkFramebuffer> &swap_chain_framebuffers,
    VkRenderPass render_pass) {
	swap_chain_framebuffers.resize(swap_chain_image_views.size());

	for (size_t i = 0; i < swap_chain_image_views.size(); i++) {
		VkImageView attachments[] = {swap_chain_image_views[i]};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = render_pass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swap_chain_extent.width;
		framebufferInfo.height = swap_chain_extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr,
		                        &swap_chain_framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void FrameBuffer::cleanup(VkDevice device,
                          std::vector<VkFramebuffer> &framebuffers) {
	for (auto framebuffer : framebuffers) {
		if (framebuffer != VK_NULL_HANDLE) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
	}
	framebuffers.clear();
}
} // namespace zenithstgv
