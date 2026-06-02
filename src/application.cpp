#include "application.h"

#include <memory>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include "main.h"
#include "vertex.h"
#include "window/window.h"

#include "vulkan_render/command.h"
#include "vulkan_render/descriptor.h"
#include "vulkan_render/device.h"
#include "vulkan_render/draw.h"
#include "vulkan_render/index_buffer.h"
#include "vulkan_render/instance.h"
#include "vulkan_render/instance_buffer.h"
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

void Application::initVulkan() {
	Instance::createInstance(instance_);

	Surface::createSurface(window_.get(), instance_, surface_);

	Device::pickPhysicalDevice(instance_.get(), physical_device_, surface_);

	Device::createLogicalDevice(device_, physical_device_, graphics_queue_,
	                            present_queue_, surface_);

	SwapChain::createSwapChain(window_.get(), device_.get(), physical_device_,
	                           surface_, swap_chain_, swap_chain_images_,
	                           swap_chain_image_format_, swap_chain_extent_);

	ImageView::createImageViews(device_.get(), swap_chain_images_,
	                            swap_chain_image_format_,
	                            swap_chain_image_views_);

	Descriptor::createDescriptorSetLayout(device_.get(),
	                                      descriptor_set_layout_);

	RenderPass::createRenderPass(device_.get(), swap_chain_image_format_,
	                             render_pass_);

	GraphicsPipeline::createGraphicsPipelines(
	    device_.get(), render_pass_.get(), descriptor_set_layout_.get(),
	    pipeline_layout_, blend_pipelines_);

	FrameBuffer::createFramebuffers(
	    device_.get(), swap_chain_extent_, swap_chain_image_views_,
	    swap_chain_framebuffers_, render_pass_.get());

	Command::createCommandPool(device_.get(), physical_device_, surface_,
	                           command_pool_);

	Command::createCommandBuffer(device_.get(), command_pool_.get(),
	                             command_buffers_);

	SyncObjects::createSyncObjects(
	    device_.get(), swap_chain_.get(), image_available_semaphores_,
	    render_finished_semaphores_, in_flight_fences_);

	VertexBuffer::createVertexBuffer(device_.get(), physical_device_, vertices_,
	                                 vertex_buffer_, vertex_buffer_memory_);

	IndexBuffer::createIndexBuffer(device_.get(), physical_device_, indices_,
	                               index_buffer_, index_buffer_memory_);

	Texture::createTextureImage(
	    device_.get(), physical_device_, command_pool_.get(), graphics_queue_,
	    "textures/test.png", texture_image_, texture_image_memory_);

	Texture::createTextureImageView(device_.get(), texture_image_.get(),
	                                texture_image_view_);

	Texture::createTextureSampler(device_.get(), physical_device_,
	                              texture_sampler_);

	Descriptor::createDescriptorPool(device_.get(), descriptor_pool_);

	Descriptor::createDescriptorSet(
	    device_.get(), descriptor_pool_.get(), descriptor_set_layout_.get(),
	    texture_image_view_.get(), texture_sampler_.get(), descriptor_set_);

	for (int i = 0; i < 4; i++) {
		InstanceBuffer::createInstanceBuffer(
		    device_.get(), physical_device_, kMaxInstances,
		    instance_buffers_[i], instance_buffer_memories_[i]);
	}
}

void Application::mainLoop() {
	bool running = true;
	double elapsed_time = 0.0;

	TimeUtl time_mng;
	time_mng.StartTimer();

	while (running) {
		time_mng.ElapsedTime();

		elapsed_time = time_mng.NSec2Double(time_mng.Timer());

		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		if (!running) {
			time_mng.RequestStop();
			break;
		}

		instance_lists_[0].clear();
		for (int i = 0; i < 100; i++) {
			Sprite spr;
			float x = (i % 10) * 0.2f - 0.9f;
			float y = (i / 10.0f) * 0.2f - 0.9f;
			spr.pos[0] = Vec2D(x, y);
			spr.pos[1] = Vec2D(x + 0.1f, y);
			spr.pos[2] = Vec2D(x + 0.1f, y + 0.1f);
			spr.pos[3] = Vec2D(x, y + 0.1f);
			spr.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
			instance_lists_[0].push_back(spr.toInstanceData());
		}

		for (int i = 0; i < 4; i++) {
			if (!instance_lists_[i].empty()) {
				InstanceBuffer::updateInstanceBuffer(
				    device_.get(), instance_buffer_memories_[i].get(),
				    instance_lists_[i]);
			}
		}

		if (Draw::drawFrame(
		        device_.get(), swap_chain_.get(), swap_chain_extent_,
		        swap_chain_framebuffers_, render_pass_.get(), blend_pipelines_,
		        pipeline_layout_.get(), command_buffers_, graphics_queue_,
		        present_queue_, image_available_semaphores_,
		        render_finished_semaphores_, in_flight_fences_, current_frame_,
		        vertex_buffer_.get(), index_buffer_.get(),
		        static_cast<uint32_t>(indices_.size()), instance_buffers_,
		        instance_lists_, descriptor_set_,
		        static_cast<float>(elapsed_time))) {

			recreateSwapChain();
		}

		current_frame_ = (current_frame_ + 1) % kMaxFramesInFlight;

		time_mng.FrameWait();
	}

	device_->waitIdle();
}

void Application::cleanup() { SDL_Quit(); }

void Application::recreateSwapChain() {
	int width = 0;
	int height = 0;

	SDL_GetWindowSizeInPixels(window_.get(), &width, &height);

	if (width == 0 || height == 0) {
		return;
	}

	device_->waitIdle();

	image_available_semaphores_.clear();
	render_finished_semaphores_.clear();
	in_flight_fences_.clear();

	swap_chain_framebuffers_.clear();
	swap_chain_image_views_.clear();

	swap_chain_.reset();

	SwapChain::createSwapChain(window_.get(), device_.get(), physical_device_,
	                           surface_, swap_chain_, swap_chain_images_,
	                           swap_chain_image_format_, swap_chain_extent_);

	ImageView::createImageViews(device_.get(), swap_chain_images_,
	                            swap_chain_image_format_,
	                            swap_chain_image_views_);

	FrameBuffer::createFramebuffers(
	    device_.get(), swap_chain_extent_, swap_chain_image_views_,
	    swap_chain_framebuffers_, render_pass_.get());

	SyncObjects::createSyncObjects(
	    device_.get(), swap_chain_.get(), image_available_semaphores_,
	    render_finished_semaphores_, in_flight_fences_);
}
} // namespace zenithstgv
