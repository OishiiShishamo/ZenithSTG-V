#include "vulkan_render/sync_objects.h"

#include <stdexcept>

#include <vulkan/vulkan.h>

namespace zenithstgv {
void SyncObjects::createSyncObjects(VkDevice device,
                                    VkSemaphore &image_available_semaphore,
                                    VkSemaphore &render_finished_semaphore,
                                    VkFence &in_flight_fence) {
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr,
	                      &image_available_semaphore) != VK_SUCCESS ||
	    vkCreateSemaphore(device, &semaphoreInfo, nullptr,
	                      &render_finished_semaphore) != VK_SUCCESS ||
	    vkCreateFence(device, &fenceInfo, nullptr, &in_flight_fence) !=
	        VK_SUCCESS) {
		throw std::runtime_error("failed to create sync objects!");
	}
}
} // namespace zenithstgv
