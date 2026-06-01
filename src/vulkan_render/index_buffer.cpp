#include "vulkan_render/index_buffer.h"

#include <cstring>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
void IndexBuffer::createIndexBuffer(
    const vk::Device &device, const vk::PhysicalDevice physical_device,
    const std::vector<uint16_t> &indices, vk::UniqueBuffer &index_buffer,
    vk::UniqueDeviceMemory &index_buffer_memory) {

	const vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	const vk::BufferCreateInfo bufferInfo{{},
	                                      bufferSize,
	                                      vk::BufferUsageFlagBits::eIndexBuffer,
	                                      vk::SharingMode::eExclusive};

	index_buffer = device.createBufferUnique(bufferInfo);

	const auto memRequirements =
	    device.getBufferMemoryRequirements(index_buffer.get());

	const vk::MemoryAllocateInfo allocInfo{
	    memRequirements.size,
	    findMemoryType(physical_device, memRequirements.memoryTypeBits,
	                   vk::MemoryPropertyFlagBits::eHostVisible |
	                       vk::MemoryPropertyFlagBits::eHostCoherent)};

	index_buffer_memory = device.allocateMemoryUnique(allocInfo);

	device.bindBufferMemory(index_buffer.get(), index_buffer_memory.get(), 0);

	void *data = device.mapMemory(index_buffer_memory.get(), 0, bufferSize);

	std::memcpy(data, indices.data(), static_cast<size_t>(bufferSize));

	device.unmapMemory(index_buffer_memory.get());
}

uint32_t IndexBuffer::findMemoryType(const vk::PhysicalDevice physical_device,
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
