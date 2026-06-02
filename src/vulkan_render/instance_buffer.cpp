#include "vulkan_render/instance_buffer.h"

#include <cstring>
#include <iostream>
#include <stdexcept>

namespace zenithstgv {

void InstanceBuffer::createInstanceBuffer(
    const vk::Device &device, const vk::PhysicalDevice physical_device,
    uint32_t max_instances, vk::UniqueBuffer &instance_buffer,
    vk::UniqueDeviceMemory &instance_buffer_memory) {

	vk::DeviceSize buffer_size = sizeof(InstanceData) * max_instances;

	vk::BufferCreateInfo bufferInfo{{},
	                                buffer_size,
	                                vk::BufferUsageFlagBits::eVertexBuffer,
	                                vk::SharingMode::eExclusive};

	instance_buffer = device.createBufferUnique(bufferInfo);

	auto memReq = device.getBufferMemoryRequirements(*instance_buffer);

	vk::MemoryAllocateInfo allocInfo{
	    memReq.size,
	    findMemoryType(physical_device, memReq.memoryTypeBits,
	                   vk::MemoryPropertyFlagBits::eHostVisible |
	                       vk::MemoryPropertyFlagBits::eHostCoherent)};

	instance_buffer_memory = device.allocateMemoryUnique(allocInfo);
	device.bindBufferMemory(*instance_buffer, *instance_buffer_memory, 0);
}

void InstanceBuffer::updateInstanceBuffer(
    const vk::Device &device, const vk::DeviceMemory instance_buffer_memory,
    const std::vector<InstanceData> &instances) {

	if (instances.empty())
		return;

	vk::DeviceSize size = sizeof(InstanceData) * instances.size();
	void *data = device.mapMemory(instance_buffer_memory, 0, size);
	memcpy(data, instances.data(), (size_t)size);
	device.unmapMemory(instance_buffer_memory);
}

uint32_t
InstanceBuffer::findMemoryType(const vk::PhysicalDevice physical_device,
                               const uint32_t type_filter,
                               const vk::MemoryPropertyFlags properties) {

	auto memProperties = physical_device.getMemoryProperties();

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
