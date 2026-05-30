#pragma once
#ifndef ZENITHSTGV_SRC_APPLICATION_H_
#define ZENITHSTGV_SRC_APPLICATION_H_

#include <memory>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "window/window.h"

namespace zenithstgv {
class Application {
  public:
	void run() {
		initWindow(window_);
		initVulkan();
		mainLoop();
		cleanup();
	}

  private:
	WindowPtr window_;
	VkInstance instance_ = VK_NULL_HANDLE;
	VkSurfaceKHR surface_;

	VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
	VkDevice device_;

	VkQueue graphics_queue_;
	VkQueue present_queue_;

	VkSwapchainKHR swap_chain_;
	std::vector<VkImage> swap_chain_images_;
	VkFormat swap_chain_image_format_;
	VkExtent2D swap_chain_extent_;
	std::vector<VkImageView> swap_chain_image_views_;
	std::vector<VkFramebuffer> swap_chain_framebuffers_;

	VkRenderPass render_pass_;
	VkPipelineLayout pipeline_layout_;
	VkPipeline graphics_pipeline_;

	VkCommandPool command_pool_;
	VkCommandBuffer command_buffer_;

	VkSemaphore image_available_semaphore_;
	VkSemaphore render_finished_semaphore_;
	VkFence in_flight_fence_;

	void initVulkan();
	void mainLoop();
	void cleanup();
};
} // namespace zenithstgv
#endif
