#include "vulkan_render/draw.h"

#include <cstdint>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "vulkan_render/command.h"

namespace zenithstgv {
bool Draw::drawFrame(
    const vk::Device &device, const vk::SwapchainKHR &swap_chain,
    vk::Extent2D swap_chain_extent,
    const std::vector<vk::UniqueFramebuffer> &swap_chain_framebuffers,
    const vk::RenderPass &render_pass,
    const std::array<vk::UniquePipeline, 4> &blend_pipelines,
    const vk::PipelineLayout &pipeline_layout,
    const std::vector<vk::CommandBuffer> &command_buffers,
    const vk::Queue &graphics_queue, const vk::Queue &present_queue,
    const std::vector<vk::UniqueSemaphore> &image_available_semaphores,
    const std::vector<vk::UniqueSemaphore> &render_finished_semaphores,
    const std::vector<vk::UniqueFence> &in_flight_fences,
    uint32_t current_frame, const vk::Buffer &vertex_buffer,
    const vk::Buffer &index_buffer, uint32_t indices_size,
    const std::array<vk::UniqueBuffer, 4> &instance_buffers,
    const std::array<std::vector<InstanceData>, 4> &instance_lists,
    const vk::DescriptorSet &descriptor_set, float elapsed_time) {

	(void)device.waitForFences(in_flight_fences[current_frame].get(), VK_TRUE,
	                           UINT64_MAX);

	device.resetFences(in_flight_fences[current_frame].get());

	auto [result, imageIndex] = device.acquireNextImageKHR(
	    swap_chain, UINT64_MAX,
	    image_available_semaphores[current_frame].get());

	if (result == vk::Result::eErrorOutOfDateKHR) {
		return true;
	}

	if (result != vk::Result::eSuccess &&
	    result != vk::Result::eSuboptimalKHR) {

		throw std::runtime_error("failed to acquire swap chain image!");
	}

	command_buffers[current_frame].reset();

	Command::recordCommandBuffer(
	    command_buffers[current_frame], imageIndex, swap_chain_extent,
	    swap_chain_framebuffers, render_pass, blend_pipelines, pipeline_layout,
	    vertex_buffer, index_buffer, indices_size, instance_buffers,
	    instance_lists, descriptor_set, elapsed_time);

	constexpr vk::PipelineStageFlags waitStages =
	    vk::PipelineStageFlagBits::eColorAttachmentOutput;

	vk::SubmitInfo submitInfo{1,
	                          &image_available_semaphores[current_frame].get(),
	                          &waitStages,
	                          1,
	                          &command_buffers[current_frame],
	                          1,
	                          &render_finished_semaphores[imageIndex].get()};

	graphics_queue.submit(submitInfo, in_flight_fences[current_frame].get());

	vk::PresentInfoKHR presentInfo{
	    1, &render_finished_semaphores[imageIndex].get(), 1, &swap_chain,
	    &imageIndex};

	result = present_queue.presentKHR(presentInfo);

	if (result == vk::Result::eErrorOutOfDateKHR ||
	    result == vk::Result::eSuboptimalKHR) {

		return true;
	}

	if (result != vk::Result::eSuccess) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	return false;
}
} // namespace zenithstgv
