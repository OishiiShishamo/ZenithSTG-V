#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_DEVICE_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_DEVICE_H_

#include <cstdint>
#include <set>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "vulkan_render/queue_family.h"

namespace zenithstgv {
class Device {
  public:
	static void pickPhysicalDevice(vk::Instance instance,
	                               vk::PhysicalDevice &physicalDevice,
	                               vk::SurfaceKHR surface);
	static void createLogicalDevice(vk::UniqueDevice &device,
	                                vk::PhysicalDevice physicalDevice,
	                                vk::Queue &graphicsQueue,
	                                vk::Queue &presentQueue,
	                                vk::SurfaceKHR surface);

  private:
	static int rateDeviceSuitability(vk::PhysicalDevice device,
	                                 vk::SurfaceKHR surface);
	static bool checkDeviceExtensionSupport(vk::PhysicalDevice device);
};
} // namespace zenithstgv

#endif
