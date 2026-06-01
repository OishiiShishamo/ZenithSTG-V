#pragma once
#ifndef ZENITHSTGV_INCLUDE_DRAW_H_
#define ZENITHSTGV_INCLUDE_DRAW_H_

#include <cstdint>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

#include "vulkan_render/command.h"

namespace zenithstgv {
class Draw {
  public:
	static bool drawFrame(
	    const vk::Device &device, const vk::SwapchainKHR &swap_chain,
	    vk::Extent2D swap_chain_extent,
	    const std::vector<vk::UniqueFramebuffer> &swap_chain_framebuffers,
	    const vk::RenderPass &render_pass,
	    const vk::Pipeline &graphics_pipeline,
	    const vk::PipelineLayout &pipeline_layout,
	    const std::vector<vk::CommandBuffer> &command_buffers,
	    const vk::Queue &graphics_queue, const vk::Queue &present_queue,
	    const std::vector<vk::UniqueSemaphore> &image_available_semaphores,
	    const std::vector<vk::UniqueSemaphore> &render_finished_semaphores,
	    const std::vector<vk::UniqueFence> &in_flight_fences,
	    uint32_t current_frame, const vk::Buffer &vertex_buffer,
	    const vk::Buffer &index_buffer, uint32_t indices_size,
	    const vk::DescriptorSet &descriptor_set, float elapsed_time);
};
} // namespace zenithstgv

#endif
