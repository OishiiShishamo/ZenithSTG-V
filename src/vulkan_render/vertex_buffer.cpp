#include "vulkan_render/vertex_buffer.h"

#include <cstring>
#include <stdexcept>

namespace zenithstgv {
void VertexBuffer::createVertexBuffer(VkDevice device,
                                      VkPhysicalDevice physical_device,
                                      const std::vector<Vertex> &vertices,
                                      VkBuffer &vertex_buffer,
                                      VkDeviceMemory &vertex_buffer_memory) {

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(vertices[0]) * vertices.size();
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertex_buffer) !=
	    VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, vertex_buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex =
	    findMemoryType(physical_device, memRequirements.memoryTypeBits,
	                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &vertex_buffer_memory) !=
	    VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(device, vertex_buffer, vertex_buffer_memory, 0);

	void *data;
	vkMapMemory(device, vertex_buffer_memory, 0, bufferInfo.size, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferInfo.size);
	vkUnmapMemory(device, vertex_buffer_memory);
}

void VertexBuffer::cleanup(VkDevice device, VkBuffer &vertex_buffer,
                           VkDeviceMemory &vertex_buffer_memory) {
	if (vertex_buffer != VK_NULL_HANDLE) {
		vkDestroyBuffer(device, vertex_buffer, nullptr);
		vertex_buffer = VK_NULL_HANDLE;
	}
	if (vertex_buffer_memory != VK_NULL_HANDLE) {
		vkFreeMemory(device, vertex_buffer_memory, nullptr);
		vertex_buffer_memory = VK_NULL_HANDLE;
	}
}

uint32_t VertexBuffer::findMemoryType(VkPhysicalDevice physical_device,
                                      uint32_t type_filter,
                                      VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physical_device, &memProperties);

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
