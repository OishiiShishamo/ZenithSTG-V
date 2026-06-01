#include "vulkan_render/texture.h"

#include <cstring>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

#include "utility/image_loader.h"

namespace zenithstgv {
void Texture::createImage(const vk::Device &device,
                          const vk::PhysicalDevice &physical_device,
                          uint32_t width, uint32_t height, vk::Format format,
                          vk::ImageTiling tiling, vk::ImageUsageFlags usage,
                          vk::MemoryPropertyFlags properties,
                          vk::UniqueImage &image,
                          vk::UniqueDeviceMemory &image_memory) {

	vk::ImageCreateInfo imageInfo{{},
	                              vk::ImageType::e2D,
	                              format,
	                              vk::Extent3D{width, height, 1},
	                              1,
	                              1,
	                              vk::SampleCountFlagBits::e1,
	                              tiling,
	                              usage,
	                              vk::SharingMode::eExclusive};

	image = device.createImageUnique(imageInfo);

	const auto memReq = device.getImageMemoryRequirements(image.get());

	vk::MemoryAllocateInfo allocInfo{
	    memReq.size,
	    findMemoryType(physical_device, memReq.memoryTypeBits, properties)};

	image_memory = device.allocateMemoryUnique(allocInfo);

	device.bindImageMemory(image.get(), image_memory.get(), 0);
}

void Texture::createTextureImage(const vk::Device &device,
                                 const vk::PhysicalDevice &physical_device,
                                 const vk::CommandPool &command_pool,
                                 const vk::Queue &graphics_queue,
                                 const std::string &path,
                                 vk::UniqueImage &texture_image,
                                 vk::UniqueDeviceMemory &texture_image_memory) {

	ImageData img = loadImage(path);

	const vk::DeviceSize imageSize = static_cast<vk::DeviceSize>(img.width) *
	                                 static_cast<vk::DeviceSize>(img.height) *
	                                 4;

	vk::BufferCreateInfo bufferInfo{{},
	                                imageSize,
	                                vk::BufferUsageFlagBits::eTransferSrc,
	                                vk::SharingMode::eExclusive};

	auto stagingBuffer = device.createBufferUnique(bufferInfo);

	const auto memReq = device.getBufferMemoryRequirements(stagingBuffer.get());

	vk::MemoryAllocateInfo allocInfo{
	    memReq.size,
	    findMemoryType(physical_device, memReq.memoryTypeBits,
	                   vk::MemoryPropertyFlagBits::eHostVisible |
	                       vk::MemoryPropertyFlagBits::eHostCoherent)};

	auto stagingBufferMemory = device.allocateMemoryUnique(allocInfo);

	device.bindBufferMemory(stagingBuffer.get(), stagingBufferMemory.get(), 0);

	void *data = device.mapMemory(stagingBufferMemory.get(), 0, imageSize);

	memcpy(data, img.pixels, static_cast<size_t>(imageSize));

	device.unmapMemory(stagingBufferMemory.get());

	img.free();

	createImage(device, physical_device, static_cast<uint32_t>(img.width),
	            static_cast<uint32_t>(img.height), vk::Format::eR8G8B8A8Srgb,
	            vk::ImageTiling::eOptimal,
	            vk::ImageUsageFlagBits::eTransferDst |
	                vk::ImageUsageFlagBits::eSampled,
	            vk::MemoryPropertyFlagBits::eDeviceLocal, texture_image,
	            texture_image_memory);

	transitionImageLayout(device, command_pool, graphics_queue,
	                      texture_image.get(), vk::ImageLayout::eUndefined,
	                      vk::ImageLayout::eTransferDstOptimal);

	copyBufferToImage(device, command_pool, graphics_queue, stagingBuffer.get(),
	                  texture_image.get(), static_cast<uint32_t>(img.width),
	                  static_cast<uint32_t>(img.height));

	transitionImageLayout(device, command_pool, graphics_queue,
	                      texture_image.get(),
	                      vk::ImageLayout::eTransferDstOptimal,
	                      vk::ImageLayout::eShaderReadOnlyOptimal);
}

void Texture::createTextureSampler(const vk::Device &device,
                                   const vk::PhysicalDevice &physical_device,
                                   vk::UniqueSampler &texture_sampler) {

	const auto properties = physical_device.getProperties();

	vk::SamplerCreateInfo samplerInfo{{},
	                                  vk::Filter::eLinear,
	                                  vk::Filter::eLinear,
	                                  vk::SamplerMipmapMode::eLinear,
	                                  vk::SamplerAddressMode::eRepeat,
	                                  vk::SamplerAddressMode::eRepeat,
	                                  vk::SamplerAddressMode::eRepeat,
	                                  0.0f,
	                                  VK_TRUE,
	                                  properties.limits.maxSamplerAnisotropy,
	                                  VK_FALSE,
	                                  vk::CompareOp::eAlways,
	                                  0.0f,
	                                  0.0f,
	                                  vk::BorderColor::eIntOpaqueBlack,
	                                  VK_FALSE};

	texture_sampler = device.createSamplerUnique(samplerInfo);
}

vk::CommandBuffer
Texture::beginSingleTimeCommands(const vk::Device &device,
                                 const vk::CommandPool &command_pool) {

	vk::CommandBufferAllocateInfo allocInfo{
	    command_pool, vk::CommandBufferLevel::ePrimary, 1};

	auto commandBuffers = device.allocateCommandBuffers(allocInfo);

	auto commandBuffer = commandBuffers[0];

	vk::CommandBufferBeginInfo beginInfo{
	    vk::CommandBufferUsageFlagBits::eOneTimeSubmit};

	commandBuffer.begin(beginInfo);

	return commandBuffer;
}

void Texture::endSingleTimeCommands(const vk::Device &device,
                                    const vk::CommandPool &command_pool,
                                    const vk::Queue &graphics_queue,
                                    const vk::CommandBuffer &command_buffer) {

	command_buffer.end();

	vk::SubmitInfo submitInfo{0, nullptr, nullptr, 1, &command_buffer};

	graphics_queue.submit(submitInfo);
	graphics_queue.waitIdle();

	device.freeCommandBuffers(command_pool, command_buffer);
}

void Texture::transitionImageLayout(const vk::Device &device,
                                    const vk::CommandPool &command_pool,
                                    const vk::Queue &graphics_queue,
                                    const vk::Image &image,
                                    vk::ImageLayout old_layout,
                                    vk::ImageLayout new_layout) {

	const auto commandBuffer = beginSingleTimeCommands(device, command_pool);

	vk::ImageMemoryBarrier barrier{
	    {},
	    {},
	    old_layout,
	    new_layout,
	    VK_QUEUE_FAMILY_IGNORED,
	    VK_QUEUE_FAMILY_IGNORED,
	    image,
	    {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

	vk::PipelineStageFlags sourceStage;
	vk::PipelineStageFlags destinationStage;

	if (old_layout == vk::ImageLayout::eUndefined &&
	    new_layout == vk::ImageLayout::eTransferDstOptimal) {

		barrier.srcAccessMask = {};
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;

		destinationStage = vk::PipelineStageFlagBits::eTransfer;

	} else if (old_layout == vk::ImageLayout::eTransferDstOptimal &&
	           new_layout == vk::ImageLayout::eShaderReadOnlyOptimal) {

		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;

		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		sourceStage = vk::PipelineStageFlagBits::eTransfer;

		destinationStage = vk::PipelineStageFlagBits::eFragmentShader;

	} else {
		throw std::runtime_error("unsupported layout transition!");
	}

	commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, nullptr,
	                              nullptr, barrier);

	endSingleTimeCommands(device, command_pool, graphics_queue, commandBuffer);
}

void Texture::copyBufferToImage(const vk::Device &device,
                                const vk::CommandPool &command_pool,
                                const vk::Queue &graphics_queue,
                                const vk::Buffer &buffer,
                                const vk::Image &image, uint32_t width,
                                uint32_t height) {

	const auto commandBuffer = beginSingleTimeCommands(device, command_pool);

	vk::BufferImageCopy region{
	    0,         0,
	    0,         {vk::ImageAspectFlagBits::eColor, 0, 0, 1},
	    {0, 0, 0}, {width, height, 1}};

	commandBuffer.copyBufferToImage(
	    buffer, image, vk::ImageLayout::eTransferDstOptimal, region);

	endSingleTimeCommands(device, command_pool, graphics_queue, commandBuffer);
}

void Texture::createTextureImageView(const vk::Device &device,
                                     const vk::Image &texture_image,
                                     vk::UniqueImageView &texture_image_view) {

	vk::ImageViewCreateInfo viewInfo{
	    {},
	    texture_image,
	    vk::ImageViewType::e2D,
	    vk::Format::eR8G8B8A8Srgb,
	    {},
	    {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

	texture_image_view = device.createImageViewUnique(viewInfo);
}

uint32_t Texture::findMemoryType(const vk::PhysicalDevice &physical_device,
                                 uint32_t type_filter,
                                 vk::MemoryPropertyFlags properties) {

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
