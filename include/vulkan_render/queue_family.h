#pragma once
#ifndef ZENITHSTGV_SRC_VULKAN_RENDER_QUEUE_FAMILY_H_
#define ZENITHSTGV_SRC_VULKAN_RENDER_QUEUE_FAMILY_H_

#include <cstdint>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

namespace zenithstgv {
struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                     VkSurfaceKHR surface);
} // namespace zenithstgv

#endif
