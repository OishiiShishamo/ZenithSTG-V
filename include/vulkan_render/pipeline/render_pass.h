#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_PIPELINE_RENDER_PASS_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_PIPELINE_RENDER_PASS_H_

#include <stdexcept>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
class RenderPass {
  public:
	static void createRenderPass(const vk::Device &device,
	                             const vk::Format swap_chain_image_format,
	                             vk::UniqueRenderPass &render_pass);
};
} // namespace zenithstgv

#endif
