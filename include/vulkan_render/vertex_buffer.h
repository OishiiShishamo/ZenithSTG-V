#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_VERTEX_BUFFER_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_VERTEX_BUFFER_H_

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vertex.h"

namespace zenithstgv {
class VertexBuffer {
  public:
	static void createVertexBuffer(
	    const vk::Device &device, const vk::PhysicalDevice physical_device,
	    const std::vector<Vertex> &vertices, vk::UniqueBuffer &vertex_buffer,
	    vk::UniqueDeviceMemory &vertex_buffer_memory);

  private:
	static uint32_t findMemoryType(const vk::PhysicalDevice physical_device,
	                               const uint32_t type_filter,
	                               const vk::MemoryPropertyFlags properties);
};
} // namespace zenithstgv

#endif
