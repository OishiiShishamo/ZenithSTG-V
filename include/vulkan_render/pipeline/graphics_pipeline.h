#pragma once
#ifndef ZENITHSTGV_SRC_VULKAN_RENDER_PIPELINE_GRAPHICS_PIPELINE_H_
#define ZENITHSTGV_SRC_VULKAN_RENDER_PIPELINE_GRAPHICS_PIPELINE_H_

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "utility/read_file.h"

namespace zenithstgv {
class GraphicsPipeline {
  public:
	static void createGraphicsPipeline(VkDevice device,
	                                   VkRenderPass render_pass,
	                                   VkPipelineLayout &pipeline_layout,
	                                   VkPipeline &graphics_pipeline);
	static VkShaderModule createShaderModule(const std::vector<char> &code,
	                                         VkDevice device);
};
} // namespace zenithstgv

#endif
