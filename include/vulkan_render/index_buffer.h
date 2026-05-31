#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_INDEX_BUFFER_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_INDEX_BUFFER_H_

#include <vector>
#include <vulkan/vulkan.h>

namespace zenithstgv {
class IndexBuffer {
  public:
	static void createIndexBuffer(VkDevice device,
	                              VkPhysicalDevice physical_device,
	                              const std::vector<uint16_t> &indices,
	                              VkBuffer &index_buffer,
	                              VkDeviceMemory &index_buffer_memory);

	static void cleanup(VkDevice device, VkBuffer &index_buffer,
	                    VkDeviceMemory &index_buffer_memory);

  private:
	static uint32_t findMemoryType(VkPhysicalDevice physical_device,
	                               uint32_t type_filter,
	                               VkMemoryPropertyFlags properties);
};
} // namespace zenithstgv

#endif
