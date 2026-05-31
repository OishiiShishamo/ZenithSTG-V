#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_VERTEX_BUFFER_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_VERTEX_BUFFER_H_

#include <vector>

#include <vulkan/vulkan.h>

#include "vertex.h"

namespace zenithstgv {
class VertexBuffer {
  public:
	static void createVertexBuffer(VkDevice device,
	                               VkPhysicalDevice physical_device,
	                               const std::vector<Vertex> &vertices,
	                               VkBuffer &vertex_buffer,
	                               VkDeviceMemory &vertex_buffer_memory);

	static void cleanup(VkDevice device, VkBuffer &vertex_buffer,
	                    VkDeviceMemory &vertex_buffer_memory);

  private:
	static uint32_t findMemoryType(VkPhysicalDevice physical_device,
	                               uint32_t type_filter,
	                               VkMemoryPropertyFlags properties);
};
} // namespace zenithstgv

#endif
