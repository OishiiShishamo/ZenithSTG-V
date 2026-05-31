#pragma once
#ifndef ZENITHSTGV_INCLUDE_DRAW_H_
#define ZENITHSTGV_INCLUDE_DRAW_H_

#include <cstdint>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "vulkan_render/command.h"

namespace zenithstgv {
class Draw {
  public:
	static bool drawFrame(VkDevice &device, VkSwapchainKHR swap_chain,
	                      VkExtent2D swap_chain_extent,
	                      std::vector<VkFramebuffer> &swap_chain_framebuffers,
	                      VkRenderPass render_pass,
	                      VkPipeline graphics_pipeline,
	                      VkPipelineLayout pipeline_layout,
	                      std::vector<VkCommandBuffer> &command_buffers,
	                      VkQueue graphics_queue, VkQueue present_queue,
	                      std::vector<VkSemaphore> &image_available_semaphores,
	                      std::vector<VkSemaphore> &render_finished_semaphores,
	                      std::vector<VkFence> &in_flight_fences,
	                      uint32_t current_frame, VkBuffer vertex_buffer,
	                      VkBuffer index_buffer, uint32_t indices_size,
	                      VkDescriptorSet descriptor_set, float elapsed_time);
};
} // namespace zenithstgv

#endif
