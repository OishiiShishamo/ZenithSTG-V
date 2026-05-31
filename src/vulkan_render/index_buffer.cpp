#include "vulkan_render/index_buffer.h"

#include <cstring>

#include <stdexcept>

namespace zenithstgv {
void IndexBuffer::createIndexBuffer(VkDevice device,
                                    VkPhysicalDevice physical_device,
                                    const std::vector<uint16_t> &indices,
                                    VkBuffer &index_buffer,
                                    VkDeviceMemory &index_buffer_memory) {

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(indices[0]) * indices.size();
	bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &index_buffer) !=
	    VK_SUCCESS) {
		throw std::runtime_error("failed to create index buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, index_buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex =
	    findMemoryType(physical_device, memRequirements.memoryTypeBits,
	                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &index_buffer_memory) !=
	    VK_SUCCESS) {
		throw std::runtime_error("failed to allocate index buffer memory!");
	}

	vkBindBufferMemory(device, index_buffer, index_buffer_memory, 0);

	void *data;
	vkMapMemory(device, index_buffer_memory, 0, bufferInfo.size, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferInfo.size);
	vkUnmapMemory(device, index_buffer_memory);
}

void IndexBuffer::cleanup(VkDevice device, VkBuffer &index_buffer,
                          VkDeviceMemory &index_buffer_memory) {
	if (index_buffer != VK_NULL_HANDLE) {
		vkDestroyBuffer(device, index_buffer, nullptr);
		index_buffer = VK_NULL_HANDLE;
	}
	if (index_buffer_memory != VK_NULL_HANDLE) {
		vkFreeMemory(device, index_buffer_memory, nullptr);
		index_buffer_memory = VK_NULL_HANDLE;
	}
}

uint32_t IndexBuffer::findMemoryType(VkPhysicalDevice physical_device,
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
