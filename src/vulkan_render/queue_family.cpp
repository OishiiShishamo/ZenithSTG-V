#include "vulkan_render/queue_family.h"

#include <cstdint>
#include <optional>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device,
                                     vk::SurfaceKHR surface) {
	QueueFamilyIndices indices;

	const auto queueFamilies = device.getQueueFamilyProperties();

	for (uint32_t i = 0; i < queueFamilies.size(); i++) {

		if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.graphicsFamily = i;
		}

		vk::Bool32 presentSupport = device.getSurfaceSupportKHR(i, surface);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}
	}

	return indices;
}
} // namespace zenithstgv
