#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_PIPELINE_GRAPHICS_PIPELINE_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_PIPELINE_GRAPHICS_PIPELINE_H_

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "utility/read_file.h"

namespace zenithstgv {
class GraphicsPipeline {
  public:
	static void
	createGraphicsPipeline(vk::Device device, vk::RenderPass render_pass,
	                       vk::DescriptorSetLayout descriptor_set_layout,
	                       vk::UniquePipelineLayout &pipeline_layout,
	                       vk::UniquePipeline &graphics_pipeline);
	static vk::UniqueShaderModule
	createShaderModule(const std::vector<char> &code, vk::Device device);
};
} // namespace zenithstgv

#endif
