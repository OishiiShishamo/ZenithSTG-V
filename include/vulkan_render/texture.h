#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_TEXTURE_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_TEXTURE_H_

#include <string>
#include <vulkan/vulkan.h>

namespace zenithstgv {
class Texture {
  public:
	static void
	createTextureImage(VkDevice device, VkPhysicalDevice physical_device,
	                   VkCommandPool command_pool, VkQueue graphics_queue,
	                   const std::string &path, VkImage &texture_image,
	                   VkDeviceMemory &texture_image_memory);

	static void createTextureImageView(VkDevice device, VkImage texture_image,
	                                   VkImageView &texture_image_view);

	static void createTextureSampler(VkDevice device,
	                                 VkPhysicalDevice physical_device,
	                                 VkSampler &texture_sampler);

	static void cleanup(VkDevice device, VkImage &texture_image,
	                    VkDeviceMemory &texture_image_memory,
	                    VkImageView &texture_image_view,
	                    VkSampler &texture_sampler);

  private:
	static void createImage(VkDevice device, VkPhysicalDevice physical_device,
	                        uint32_t width, uint32_t height, VkFormat format,
	                        VkImageTiling tiling, VkImageUsageFlags usage,
	                        VkMemoryPropertyFlags properties, VkImage &image,
	                        VkDeviceMemory &image_memory);

	static void transitionImageLayout(VkDevice device,
	                                  VkCommandPool command_pool,
	                                  VkQueue graphics_queue, VkImage image,
	                                  VkImageLayout old_layout,
	                                  VkImageLayout new_layout);

	static void copyBufferToImage(VkDevice device, VkCommandPool command_pool,
	                              VkQueue graphics_queue, VkBuffer buffer,
	                              VkImage image, uint32_t width,
	                              uint32_t height);

	static VkCommandBuffer beginSingleTimeCommands(VkDevice device,
	                                               VkCommandPool command_pool);

	static void endSingleTimeCommands(VkDevice device,
	                                  VkCommandPool command_pool,
	                                  VkQueue graphics_queue,
	                                  VkCommandBuffer command_buffer);

	static uint32_t findMemoryType(VkPhysicalDevice physical_device,
	                               uint32_t type_filter,
	                               VkMemoryPropertyFlags properties);
};
} // namespace zenithstgv

#endif
