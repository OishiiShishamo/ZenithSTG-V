#include "application.h"

#include <memory>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "main.h"
#include "window/window.h"

#include "vulkan_render/command.h"
#include "vulkan_render/device.h"
#include "vulkan_render/draw.h"
#include "vulkan_render/instance.h"
#include "vulkan_render/pipeline/graphics_pipeline.h"
#include "vulkan_render/pipeline/render_pass.h"
#include "vulkan_render/surface.h"
#include "vulkan_render/swapchain/frame_buffer.h"
#include "vulkan_render/swapchain/image_view.h"
#include "vulkan_render/swapchain/swap_chain.h"
#include "vulkan_render/sync_objects.h"

namespace zenithstgv {

void Application::initVulkan() {
	Instance::createInstance(instance_);
	Surface::createSurface(window_.get(), instance_, surface_);
	Device::pickPhysicalDevice(instance_, physical_device_, surface_);
	Device::createLogicalDevice(device_, physical_device_, graphics_queue_,
	                            present_queue_, surface_);
	SwapChain::createSwapChain(window_.get(), device_, physical_device_,
	                           surface_, swap_chain_, swap_chain_images_,
	                           swap_chain_image_format_, swap_chain_extent_);
	ImageView::createImageViews(device_, swap_chain_images_,
	                            swap_chain_image_format_,
	                            swap_chain_image_views_);
	RenderPass::createRenderPass(device_, swap_chain_image_format_,
	                             render_pass_);
	GraphicsPipeline::createGraphicsPipeline(
	    device_, render_pass_, pipeline_layout_, graphics_pipeline_);
	FrameBuffer::createFramebuffers(device_, swap_chain_extent_,
	                                swap_chain_image_views_,
	                                swap_chain_framebuffers_, render_pass_);
	Command::createCommandPool(device_, physical_device_, surface_,
	                           command_pool_);
	Command::createCommandBuffer(device_, command_pool_, command_buffers_);
	SyncObjects::createSyncObjects(
	    device_, swap_chain_, image_available_semaphores_,
	    render_finished_semaphores_, in_flight_fences_);
}

void Application::mainLoop() {
	bool running = true;

	while (running) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		if (!running)
			break;

		if (Draw::drawFrame(
		        device_, swap_chain_, swap_chain_extent_,
		        swap_chain_framebuffers_, render_pass_, graphics_pipeline_,
		        command_buffers_, graphics_queue_, present_queue_,
		        image_available_semaphores_, render_finished_semaphores_,
		        in_flight_fences_, current_frame_)) {
			recreateSwapChain();
		}
		current_frame_ = (current_frame_ + 1) % kMaxFramesInFlight;
	}

	vkDeviceWaitIdle(device_);
}

void Application::cleanup() {
	for (auto X : image_available_semaphores_)
		vkDestroySemaphore(device_, X, nullptr);
	for (auto X : render_finished_semaphores_)
		vkDestroySemaphore(device_, X, nullptr);
	for (auto X : in_flight_fences_)
		vkDestroyFence(device_, X, nullptr);
	vkDestroyCommandPool(device_, command_pool_, nullptr);

	for (auto framebuffer : swap_chain_framebuffers_) {
		vkDestroyFramebuffer(device_, framebuffer, nullptr);
	}

	vkDestroyPipeline(device_, graphics_pipeline_, nullptr);
	vkDestroyPipelineLayout(device_, pipeline_layout_, nullptr);
	vkDestroyRenderPass(device_, render_pass_, nullptr);

	for (auto imageView : swap_chain_image_views_) {
		if (imageView != VK_NULL_HANDLE) {
			vkDestroyImageView(device_, imageView, nullptr);
		}
	}

	if (swap_chain_ != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(device_, swap_chain_, nullptr);
	}

	if (device_ != VK_NULL_HANDLE) {
		vkDestroyDevice(device_, nullptr);
	}

	if (surface_ != VK_NULL_HANDLE) {
		vkDestroySurfaceKHR(instance_, surface_, nullptr);
		surface_ = VK_NULL_HANDLE;
	}

	if (instance_ != VK_NULL_HANDLE) {
		vkDestroyInstance(instance_, nullptr);
		instance_ = VK_NULL_HANDLE;
	}
	SDL_Quit();
}

void Application::recreateSwapChain() {
	int width = 0, height = 0;
	SDL_GetWindowSizeInPixels(window_.get(), &width, &height);
	if (width == 0 || height == 0)
		return;

	vkDeviceWaitIdle(device_);

	for (auto s : image_available_semaphores_)
		vkDestroySemaphore(device_, s, nullptr);
	for (auto s : render_finished_semaphores_)
		vkDestroySemaphore(device_, s, nullptr);
	for (auto f : in_flight_fences_)
		vkDestroyFence(device_, f, nullptr);
	image_available_semaphores_.clear();
	render_finished_semaphores_.clear();
	in_flight_fences_.clear();

	FrameBuffer::cleanup(device_, swap_chain_framebuffers_);
	ImageView::cleanup(device_, swap_chain_image_views_);
	SwapChain::cleanup(device_, swap_chain_);

	SwapChain::createSwapChain(window_.get(), device_, physical_device_,
	                           surface_, swap_chain_, swap_chain_images_,
	                           swap_chain_image_format_, swap_chain_extent_);
	ImageView::createImageViews(device_, swap_chain_images_,
	                            swap_chain_image_format_,
	                            swap_chain_image_views_);
	FrameBuffer::createFramebuffers(device_, swap_chain_extent_,
	                                swap_chain_image_views_,
	                                swap_chain_framebuffers_, render_pass_);
	SyncObjects::createSyncObjects(
	    device_, swap_chain_, image_available_semaphores_,
	    render_finished_semaphores_, in_flight_fences_);
}
} // namespace zenithstgv
