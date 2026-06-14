#include "vulkan_render/descriptor.h"

#include <stdexcept>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
void Descriptor::createDescriptorSetLayout(
    const vk::Device &device,
    vk::UniqueDescriptorSetLayout &descriptor_set_layout) {

	const vk::DescriptorSetLayoutBinding samplerLayoutBinding{
	    0, vk::DescriptorType::eCombinedImageSampler, 1,
	    vk::ShaderStageFlagBits::eFragment};

	const vk::DescriptorSetLayoutCreateInfo layoutInfo{
	    {}, 1, &samplerLayoutBinding};

	descriptor_set_layout = device.createDescriptorSetLayoutUnique(layoutInfo);
}

void Descriptor::createFontDescriptorSetLayout(
    const vk::Device &device,
    vk::UniqueDescriptorSetLayout &font_descriptor_set_layout) {

	const vk::DescriptorSetLayoutBinding samplerLayoutBinding{
	    0, vk::DescriptorType::eCombinedImageSampler, 1,
	    vk::ShaderStageFlagBits::eFragment};

	const vk::DescriptorSetLayoutCreateInfo layoutInfo{
	    {}, 1, &samplerLayoutBinding};

	font_descriptor_set_layout =
	    device.createDescriptorSetLayoutUnique(layoutInfo);
}

void Descriptor::createDescriptorPool(
    const vk::Device &device, vk::UniqueDescriptorPool &descriptor_pool) {

	const vk::DescriptorPoolSize poolSize{
	    vk::DescriptorType::eCombinedImageSampler, 2};

	const vk::DescriptorPoolCreateInfo poolInfo{{}, 2, 1, &poolSize};

	descriptor_pool = device.createDescriptorPoolUnique(poolInfo);
}

void Descriptor::createDescriptorSet(
    const vk::Device &device, const vk::DescriptorPool descriptor_pool,
    const vk::DescriptorSetLayout descriptor_set_layout,
    const vk::ImageView texture_image_view, const vk::Sampler texture_sampler,
    vk::DescriptorSet &descriptor_set) {

	const vk::DescriptorSetAllocateInfo allocInfo{descriptor_pool, 1,
	                                              &descriptor_set_layout};

	descriptor_set = device.allocateDescriptorSets(allocInfo)[0];

	const vk::DescriptorImageInfo imageInfo{
	    texture_sampler, texture_image_view,
	    vk::ImageLayout::eShaderReadOnlyOptimal};

	const vk::WriteDescriptorSet descriptorWrite{
	    descriptor_set, 0, 0, 1, vk::DescriptorType::eCombinedImageSampler,
	    &imageInfo};

	device.updateDescriptorSets(descriptorWrite, nullptr);
}

void Descriptor::createFontDescriptorSet(
    const vk::Device &device, const vk::DescriptorPool descriptor_pool,
    const vk::DescriptorSetLayout font_descriptor_set_layout,
    const vk::ImageView font_texture_image_view,
    const vk::Sampler font_texture_sampler,
    vk::DescriptorSet &font_descriptor_set) {

	const vk::DescriptorSetAllocateInfo allocInfo{descriptor_pool, 1,
	                                              &font_descriptor_set_layout};

	font_descriptor_set = device.allocateDescriptorSets(allocInfo)[0];

	const vk::DescriptorImageInfo imageInfo{
	    font_texture_sampler, font_texture_image_view,
	    vk::ImageLayout::eShaderReadOnlyOptimal};

	const vk::WriteDescriptorSet descriptorWrite{
	    font_descriptor_set, 0, 0, 1, vk::DescriptorType::eCombinedImageSampler,
	    &imageInfo};

	device.updateDescriptorSets(descriptorWrite, nullptr);
}
} // namespace zenithstgv
