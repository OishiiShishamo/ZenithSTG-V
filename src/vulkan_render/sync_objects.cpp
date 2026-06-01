#include "vulkan_render/sync_objects.h"

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "main.h"

namespace zenithstgv {
void SyncObjects::createSyncObjects(
    const vk::Device &device, const vk::SwapchainKHR swap_chain,
    std::vector<vk::UniqueSemaphore> &image_available_semaphores,
    std::vector<vk::UniqueSemaphore> &render_finished_semaphores,
    std::vector<vk::UniqueFence> &in_flight_fences) {

	const auto swap_chain_images = device.getSwapchainImagesKHR(swap_chain);

	const uint32_t imageCount = static_cast<uint32_t>(swap_chain_images.size());

	image_available_semaphores.resize(kMaxFramesInFlight);
	render_finished_semaphores.resize(imageCount);
	in_flight_fences.resize(kMaxFramesInFlight);

	const vk::SemaphoreCreateInfo semaphoreInfo{};

	const vk::FenceCreateInfo fenceInfo{vk::FenceCreateFlagBits::eSignaled};

	for (int i = 0; i < kMaxFramesInFlight; i++) {
		image_available_semaphores[i] =
		    device.createSemaphoreUnique(semaphoreInfo);
	}

	for (uint32_t i = 0; i < imageCount; i++) {
		render_finished_semaphores[i] =
		    device.createSemaphoreUnique(semaphoreInfo);
	}

	for (int i = 0; i < kMaxFramesInFlight; i++) {
		in_flight_fences[i] = device.createFenceUnique(fenceInfo);
	}
}
} // namespace zenithstgv
