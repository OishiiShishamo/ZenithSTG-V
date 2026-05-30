#include "vulkan_render/sync_objects.h"

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "main.h"

namespace zenithstgv {
void SyncObjects::createSyncObjects(
    VkDevice device, VkSwapchainKHR swap_chain,
    std::vector<VkSemaphore> &image_available_semaphores,
    std::vector<VkSemaphore> &render_finished_semaphores,
    std::vector<VkFence> &in_flight_fences) {

	uint32_t imageCount;
	vkGetSwapchainImagesKHR(device, swap_chain, &imageCount, nullptr);

	image_available_semaphores.resize(kMaxFramesInFlight);
	render_finished_semaphores.resize(imageCount);
	in_flight_fences.resize(kMaxFramesInFlight);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (int i = 0; i < kMaxFramesInFlight; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr,
		                      &image_available_semaphores[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create sync objects!");
		}
	}
	for (uint32_t i = 0; i < imageCount; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr,
		                      &render_finished_semaphores[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create sync objects!");
		}
	}
	for (int i = 0; i < kMaxFramesInFlight; i++) {
		if (vkCreateFence(device, &fenceInfo, nullptr, &in_flight_fences[i]) !=
		    VK_SUCCESS) {
			throw std::runtime_error("failed to create sync objects!");
		}
	}
}
} // namespace zenithstgv
