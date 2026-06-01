#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_DESCRIPTOR_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_DESCRIPTOR_H_

#include <vector>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
class Descriptor {
  public:
	static void createDescriptorSetLayout(
	    const vk::Device &device,
	    vk::UniqueDescriptorSetLayout &descriptor_set_layout);

	static void createDescriptorPool(const vk::Device &device,
	                                 vk::UniqueDescriptorPool &descriptor_pool);

	static void createDescriptorSet(
	    const vk::Device &device, const vk::DescriptorPool descriptor_pool,
	    const vk::DescriptorSetLayout descriptor_set_layout,
	    const vk::ImageView texture_image_view,
	    const vk::Sampler texture_sampler, vk::DescriptorSet &descriptor_set);
};
} // namespace zenithstgv

#endif
