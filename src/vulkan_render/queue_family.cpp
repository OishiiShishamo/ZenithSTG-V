#include "vulkan_render/queue_family.h"

#include <cstdint>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

namespace zenithstgv {
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                     VkSurfaceKHR surface) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
	                                         nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
	                                         queueFamilies.data());

	for (uint32_t i = 0; i < queueFamilyCount; i++) {
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface,
		                                     &presentSupport);
		if (presentSupport)
			indices.presentFamily = i;

		if (indices.isComplete())
			break;
	}

	return indices;
}
} // namespace zenithstgv
