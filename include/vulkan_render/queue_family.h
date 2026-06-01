#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_QUEUE_FAMILY_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_QUEUE_FAMILY_H_

#include <cstdint>
#include <optional>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() const noexcept {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device,
                                     vk::SurfaceKHR surface);
} // namespace zenithstgv

#endif
