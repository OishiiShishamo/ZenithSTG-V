#pragma once
#ifndef ZENITHSTGV_SRC_VULKAN_RENDER_PIPELINE_RENDER_PASS_H_
#define ZENITHSTGV_SRC_VULKAN_RENDER_PIPELINE_RENDER_PASS_H_

#include <stdexcept>

#include <vulkan/vulkan.h>

namespace zenithstgv {
class RenderPass {
  public:
	static void createRenderPass(VkDevice device,
	                             VkFormat swap_chain_image_format,
	                             VkRenderPass &render_pass);
};
} // namespace zenithstgv

#endif
