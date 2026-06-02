#pragma once
#ifndef ZENITHSTGV_INCLUDE_COMMAND_H_
#define ZENITHSTGV_INCLUDE_COMMAND_H_

#include <stdexcept>

#include <vulkan/vulkan.h>

#include "instance_data.h"
#include "vulkan_render/queue_family.h"

namespace zenithstgv {
class Command {
  public:
	static void createCommandPool(const vk::Device &device,
	                              const vk::PhysicalDevice physical_device,
	                              const vk::SurfaceKHR surface,
	                              vk::UniqueCommandPool &command_pool);
	static void
	createCommandBuffer(const vk::Device &device,
	                    const vk::CommandPool command_pool,
	                    std::vector<vk::CommandBuffer> &command_buffers);
	static void recordCommandBuffer(
	    const vk::CommandBuffer command_buffer, const uint32_t image_index,
	    const vk::Extent2D swap_chain_extent,
	    const std::vector<vk::UniqueFramebuffer> &swap_chain_framebuffers,
	    const vk::RenderPass render_pass,
	    const std::array<vk::UniquePipeline, 4> &blend_pipelines,
	    const vk::PipelineLayout pipeline_layout,
	    const vk::Buffer vertex_buffer, const vk::Buffer index_buffer,
	    const uint32_t indices_size,
	    const std::array<vk::UniqueBuffer, 4> &instance_buffers,
	    const std::array<std::vector<InstanceData>, 4> &instance_lists,
	    const vk::DescriptorSet descriptor_set, const float elapsed_time);
};
} // namespace zenithstgv

#endif
