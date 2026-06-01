#include "vulkan_render/vertex_buffer.h"

#include <cstring>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
void VertexBuffer::createVertexBuffer(
    const vk::Device &device, const vk::PhysicalDevice physical_device,
    const std::vector<Vertex> &vertices, vk::UniqueBuffer &vertex_buffer,
    vk::UniqueDeviceMemory &vertex_buffer_memory) {

	const vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	const vk::BufferCreateInfo bufferInfo{
	    {},
	    bufferSize,
	    vk::BufferUsageFlagBits::eVertexBuffer,
	    vk::SharingMode::eExclusive};

	vertex_buffer = device.createBufferUnique(bufferInfo);

	const auto memRequirements =
	    device.getBufferMemoryRequirements(vertex_buffer.get());

	const vk::MemoryAllocateInfo allocInfo{
	    memRequirements.size,
	    findMemoryType(physical_device, memRequirements.memoryTypeBits,
	                   vk::MemoryPropertyFlagBits::eHostVisible |
	                       vk::MemoryPropertyFlagBits::eHostCoherent)};

	vertex_buffer_memory = device.allocateMemoryUnique(allocInfo);

	device.bindBufferMemory(vertex_buffer.get(), vertex_buffer_memory.get(), 0);

	void *data = device.mapMemory(vertex_buffer_memory.get(), 0, bufferSize);

	std::memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));

	device.unmapMemory(vertex_buffer_memory.get());
}

uint32_t
VertexBuffer::findMemoryType(const vk::PhysicalDevice physical_device,
                             const uint32_t type_filter,
                             const vk::MemoryPropertyFlags properties) {

	const auto memProperties = physical_device.getMemoryProperties();

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((type_filter & (1 << i)) &&
		    (memProperties.memoryTypes[i].propertyFlags & properties) ==
		        properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}
} // namespace zenithstgv
