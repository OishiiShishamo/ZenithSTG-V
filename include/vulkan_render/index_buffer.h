#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_INDEX_BUFFER_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_INDEX_BUFFER_H_

#include <vector>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
class IndexBuffer {
  public:
	static void createIndexBuffer(const vk::Device &device,
	                              const vk::PhysicalDevice physical_device,
	                              const std::vector<uint16_t> &indices,
	                              vk::UniqueBuffer &index_buffer,
	                              vk::UniqueDeviceMemory &index_buffer_memory);

  private:
	static uint32_t findMemoryType(const vk::PhysicalDevice physical_device,
	                               const uint32_t type_filter,
	                               const vk::MemoryPropertyFlags properties);
};
} // namespace zenithstgv

#endif
