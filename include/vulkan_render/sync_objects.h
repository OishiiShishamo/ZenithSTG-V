#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_SYNC_OBJECTS_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_SYNC_OBJECTS_H_

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "main.h"

namespace zenithstgv {
class SyncObjects {
  public:
	static void createSyncObjects(
	    const vk::Device &device, const vk::SwapchainKHR swap_chain,
	    std::vector<vk::UniqueSemaphore> &image_available_semaphores,
	    std::vector<vk::UniqueSemaphore> &render_finished_semaphores,
	    std::vector<vk::UniqueFence> &in_flight_fences);
};
} // namespace zenithstgv

#endif
