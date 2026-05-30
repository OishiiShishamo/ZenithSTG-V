#pragma once
#ifndef ZENITHSTGV_SRC_DRAW_H_
#define ZENITHSTGV_SRC_DRAW_H_

#include <cstdint>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "vulkan_render/command.h"

namespace zenithstgv {
class Draw {
  public:
	static void
	drawFrame(VkDevice &device, VkSwapchainKHR swap_chain,
	          VkExtent2D swap_chain_extent,
	          std::vector<VkFramebuffer> &swap_chain_framebuffers,
	          VkRenderPass render_pass, VkPipeline graphics_pipeline,
	          VkCommandBuffer &command_buffer, VkQueue graphics_queue,
	          VkQueue present_queue, VkSemaphore image_available_semaphore,
	          VkSemaphore render_finished_semaphore, VkFence &in_flight_fence);
};
} // namespace zenithstgv

#endif
