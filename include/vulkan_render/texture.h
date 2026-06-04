#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_TEXTURE_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_TEXTURE_H_

#include <string>

#include <vulkan/vulkan.hpp>

#include "utility/image_loader.h"

namespace zenithstgv {
class Texture {
  public:
	static void createTextureImage(
	    const vk::Device &device, const vk::PhysicalDevice &physical_device,
	    const vk::CommandPool &command_pool, const vk::Queue &graphics_queue,
	    const ImageData &atlas, vk::UniqueImage &texture_image,
	    vk::UniqueDeviceMemory &texture_image_memory);

	static void createTextureImageView(const vk::Device &device,
	                                   const vk::Image &texture_image,
	                                   vk::UniqueImageView &texture_image_view);

	static void createTextureSampler(const vk::Device &device,
	                                 const vk::PhysicalDevice &physical_device,
	                                 vk::UniqueSampler &texture_sampler);

  private:
	static void createImage(const vk::Device &device,
	                        const vk::PhysicalDevice &physical_device,
	                        uint32_t width, uint32_t height, vk::Format format,
	                        vk::ImageTiling tiling, vk::ImageUsageFlags usage,
	                        vk::MemoryPropertyFlags properties,
	                        vk::UniqueImage &image,
	                        vk::UniqueDeviceMemory &image_memory);

	static void transitionImageLayout(const vk::Device &device,
	                                  const vk::CommandPool &command_pool,
	                                  const vk::Queue &graphics_queue,
	                                  const vk::Image &image,
	                                  vk::ImageLayout old_layout,
	                                  vk::ImageLayout new_layout);

	static void copyBufferToImage(const vk::Device &device,
	                              const vk::CommandPool &command_pool,
	                              const vk::Queue &graphics_queue,
	                              const vk::Buffer &buffer,
	                              const vk::Image &image, uint32_t width,
	                              uint32_t height);

	static vk::CommandBuffer
	beginSingleTimeCommands(const vk::Device &device,
	                        const vk::CommandPool &command_pool);

	static void endSingleTimeCommands(const vk::Device &device,
	                                  const vk::CommandPool &command_pool,
	                                  const vk::Queue &graphics_queue,
	                                  const vk::CommandBuffer &command_buffer);

	static uint32_t findMemoryType(const vk::PhysicalDevice &physical_device,
	                               uint32_t type_filter,
	                               vk::MemoryPropertyFlags properties);
};
} // namespace zenithstgv

#endif
