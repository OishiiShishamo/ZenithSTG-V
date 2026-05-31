#pragma once
#ifndef ZENITHSTGV_INCLUDE_APPLICATION_H_
#define ZENITHSTGV_INCLUDE_APPLICATION_H_

#include <memory>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "vertex.h"
#include "window/window.h"

#include "vulkan_render/command.h"
#include "vulkan_render/descriptor.h"
#include "vulkan_render/device.h"
#include "vulkan_render/draw.h"
#include "vulkan_render/index_buffer.h"
#include "vulkan_render/instance.h"
#include "vulkan_render/pipeline/graphics_pipeline.h"
#include "vulkan_render/pipeline/render_pass.h"
#include "vulkan_render/surface.h"
#include "vulkan_render/swapchain/frame_buffer.h"
#include "vulkan_render/swapchain/image_view.h"
#include "vulkan_render/swapchain/swap_chain.h"
#include "vulkan_render/sync_objects.h"
#include "vulkan_render/texture.h"
#include "vulkan_render/vertex_buffer.h"

#include "utility/time_utl.h"

namespace zenithstgv {
class Application {
  public:
	void run() {
		initWindow(window_);
		initVulkan();
		TimeUtl::InitTimeUtl();
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

	std::vector<Vertex> vertices_ = {
	    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

	std::vector<uint16_t> indices_ = {0, 1, 2, 2, 3, 0};

	VkBuffer vertex_buffer_ = VK_NULL_HANDLE;
	VkDeviceMemory vertex_buffer_memory_ = VK_NULL_HANDLE;
	VkBuffer index_buffer_ = VK_NULL_HANDLE;
	VkDeviceMemory index_buffer_memory_ = VK_NULL_HANDLE;

	VkImage texture_image_ = VK_NULL_HANDLE;
	VkDeviceMemory texture_image_memory_ = VK_NULL_HANDLE;
	VkImageView texture_image_view_ = VK_NULL_HANDLE;
	VkSampler texture_sampler_ = VK_NULL_HANDLE;

	VkDescriptorSetLayout descriptor_set_layout_ = VK_NULL_HANDLE;
	VkDescriptorPool descriptor_pool_ = VK_NULL_HANDLE;
	VkDescriptorSet descriptor_set_ = VK_NULL_HANDLE;

	void initVulkan();
	void mainLoop();
	void cleanup();
	void recreateSwapChain();
};
} // namespace zenithstgv
#endif
