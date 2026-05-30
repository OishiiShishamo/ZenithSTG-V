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
	std::vector<VkCommandBuffer> command_buffers_;

	std::vector<VkSemaphore> image_available_semaphores_;
	std::vector<VkSemaphore> render_finished_semaphores_;
	std::vector<VkFence> in_flight_fences_;
	uint32_t current_frame_ = 0;

	void initVulkan();
	void mainLoop();
	void cleanup();
	void recreateSwapChain();
};
} // namespace zenithstgv
#endif
