#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_SYNC_OBJECTS_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_SYNC_OBJECTS_H_

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "main.h"

namespace zenithstgv {
class SyncObjects {
  public:
	static void
	createSyncObjects(VkDevice device, VkSwapchainKHR swap_chain,
	                  std::vector<VkSemaphore> &image_available_semaphores,
	                  std::vector<VkSemaphore> &render_finished_semaphores,
	                  std::vector<VkFence> &in_flight_fences);
};
} // namespace zenithstgv

#endif
