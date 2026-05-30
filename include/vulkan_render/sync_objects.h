#pragma once
#ifndef ZENITHSTGV_SRC_VULKAN_RENDER_SYNC_OBJECTS_H_
#define ZENITHSTGV_SRC_VULKAN_RENDER_SYNC_OBJECTS_H_

#include <stdexcept>

#include <vulkan/vulkan.h>

namespace zenithstgv {
class SyncObjects {
  public:
	static void createSyncObjects(VkDevice device,
	                              VkSemaphore &image_available_semaphore,
	                              VkSemaphore &render_finished_semaphore,
	                              VkFence &in_flight_fence);
};
} // namespace zenithstgv

#endif
