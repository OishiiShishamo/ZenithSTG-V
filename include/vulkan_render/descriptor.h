#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_DESCRIPTOR_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_DESCRIPTOR_H_

#include <vector>

#include <vulkan/vulkan.h>

namespace zenithstgv {
class Descriptor {
  public:
	static void
	createDescriptorSetLayout(VkDevice device,
	                          VkDescriptorSetLayout &descriptor_set_layout);

	static void createDescriptorPool(VkDevice device,
	                                 VkDescriptorPool &descriptor_pool);

	static void createDescriptorSet(VkDevice device,
	                                VkDescriptorPool descriptor_pool,
	                                VkDescriptorSetLayout descriptor_set_layout,
	                                VkImageView texture_image_view,
	                                VkSampler texture_sampler,
	                                VkDescriptorSet &descriptor_set);

	static void cleanup(VkDevice device, VkDescriptorPool &descriptor_pool,
	                    VkDescriptorSetLayout &descriptor_set_layout);
};
} // namespace zenithstgv

#endif
