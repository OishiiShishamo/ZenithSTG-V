#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_DEVICE_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_DEVICE_H_

#include <cstdint>
#include <set>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "vulkan_render/queue_family.h"

namespace zenithstgv {
class Device {
  public:
	static void pickPhysicalDevice(VkInstance instance,
	                               VkPhysicalDevice &physical_device,
	                               VkSurfaceKHR &surface);
	static void createLogicalDevice(VkDevice &device,
	                                VkPhysicalDevice &physical_device,
	                                VkQueue &graphics_queue,
	                                VkQueue &present_queue,
	                                VkSurfaceKHR surface);

  private:
	static int rateDeviceSuitability(VkPhysicalDevice device,
	                                 VkSurfaceKHR &surface);
	static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
};
} // namespace zenithstgv

#endif
