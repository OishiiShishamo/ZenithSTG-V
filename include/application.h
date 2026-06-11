#pragma once
#ifndef ZENITHSTGV_INCLUDE_APPLICATION_H_
#define ZENITHSTGV_INCLUDE_APPLICATION_H_

#include <memory>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include "instance_data.h"
#include "texture_init.h"
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

#include "utility/atlas_builder.h"
#include "utility/keyboard.h"
#include "utility/time_utl.h"

#include "object/bullet.h"
#include "object/laser.h"
#include "object/object.h"

namespace zenithstgv {
class Application {
  public:
	void run() {
		initWindow(window_);
		TextureInit::TexturesInit(atlas_);
		bullet_manager_ = std::make_unique<BulletManager>();
		laser_manager_ = std::make_unique<LaserManager>();
		bullet_manager_->InitManager();
		laser_manager_->InitManager();
		InitKeyboard(kb_);
		initVulkan();
		mainLoop();
		cleanup();
	}

  private:
	WindowPtr window_;
	vk::UniqueInstance instance_;

	vk::UniqueSurfaceKHR surface_;

	vk::PhysicalDevice physical_device_;
	vk::UniqueDevice device_;
	vk::Queue graphics_queue_;
	vk::Queue present_queue_;

	vk::UniqueSwapchainKHR swap_chain_;
	std::vector<vk::Image> swap_chain_images_;
	vk::Format swap_chain_image_format_;
	vk::Extent2D swap_chain_extent_;
	std::vector<vk::UniqueImageView> swap_chain_image_views_;
	std::vector<vk::UniqueFramebuffer> swap_chain_framebuffers_;

	vk::UniqueRenderPass render_pass_;
	vk::UniquePipelineLayout pipeline_layout_;
	std::array<vk::UniquePipeline, 4> blend_pipelines_;

	vk::UniqueCommandPool command_pool_;
	std::vector<vk::CommandBuffer> command_buffers_;

	std::vector<vk::UniqueSemaphore> image_available_semaphores_;
	std::vector<vk::UniqueSemaphore> render_finished_semaphores_;
	std::vector<vk::UniqueFence> in_flight_fences_;
	uint32_t current_frame_ = 0;

	std::vector<Vertex> vertices_ = {
	    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

	std::vector<uint16_t> indices_ = {0, 1, 2, 0, 2, 3};

	vk::UniqueBuffer vertex_buffer_;
	vk::UniqueDeviceMemory vertex_buffer_memory_;
	vk::UniqueBuffer index_buffer_;
	vk::UniqueDeviceMemory index_buffer_memory_;

	std::array<vk::UniqueBuffer, 4> instance_buffers_;
	std::array<vk::UniqueDeviceMemory, 4> instance_buffer_memories_;

	std::array<std::vector<InstanceData>, 4> instance_lists_;

	vk::UniqueImage texture_image_;
	vk::UniqueDeviceMemory texture_image_memory_;
	vk::UniqueImageView texture_image_view_;
	vk::UniqueSampler texture_sampler_;

	vk::UniqueDescriptorSetLayout descriptor_set_layout_;
	vk::UniqueDescriptorPool descriptor_pool_;
	vk::DescriptorSet descriptor_set_;

	AtlasBuilder atlas_ = AtlasBuilder(2048, 2048);

	Keyboard kb_;

	Player player_;

	std::unique_ptr<BulletManager> bullet_manager_;
	std::unique_ptr<LaserManager> laser_manager_;

	void initVulkan();
	void mainLoop();
	void cleanup();
	void recreateSwapChain();
};
} // namespace zenithstgv
#endif
