#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_INSTANCE_BUFFER_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_INSTANCE_BUFFER_H_

#include <vector>

#include <vulkan/vulkan.hpp>

#include "instance_data.h"

namespace zenithstgv {
class InstanceBuffer {
  public:
	static void createInstanceBuffer(
	    const vk::Device &device, const vk::PhysicalDevice physical_device,
	    uint32_t max_instances, vk::UniqueBuffer &instance_buffer,
	    vk::UniqueDeviceMemory &instance_buffer_memory);

	static void
	updateInstanceBuffer(const vk::Device &device,
	                     const vk::DeviceMemory instance_buffer_memory,
	                     const std::vector<InstanceData> &instances);

  private:
	static uint32_t findMemoryType(const vk::PhysicalDevice physical_device,
	                               const uint32_t type_filter,
	                               const vk::MemoryPropertyFlags properties);
};
} // namespace zenithstgv
#endif
