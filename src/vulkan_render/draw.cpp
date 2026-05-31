#include "vulkan_render/draw.h"

#include <cstdint>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "vulkan_render/command.h"

namespace zenithstgv {
bool Draw::drawFrame(VkDevice &device, VkSwapchainKHR swap_chain,
                     VkExtent2D swap_chain_extent,
                     std::vector<VkFramebuffer> &swap_chain_framebuffers,
                     VkRenderPass render_pass, VkPipeline graphics_pipeline,
                     VkPipelineLayout pipeline_layout,
                     std::vector<VkCommandBuffer> &command_buffers,
                     VkQueue graphics_queue, VkQueue present_queue,
                     std::vector<VkSemaphore> &image_available_semaphores,
                     std::vector<VkSemaphore> &render_finished_semaphores,
                     std::vector<VkFence> &in_flight_fences,
                     uint32_t current_frame, VkBuffer vertex_buffer,
                     VkBuffer index_buffer, uint32_t indices_size,
                     VkDescriptorSet descriptor_set, float elapsed_time) {

	vkWaitForFences(device, 1, &in_flight_fences[current_frame], VK_TRUE,
	                UINT64_MAX);
	vkResetFences(device, 1, &in_flight_fences[current_frame]);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(
	    device, swap_chain, UINT64_MAX,
	    image_available_semaphores[current_frame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
		return true;
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("failed to acquire swap chain image!");

	vkResetCommandBuffer(command_buffers[current_frame], 0);
	Command::recordCommandBuffer(command_buffers[current_frame], imageIndex,
	                             swap_chain_extent, swap_chain_framebuffers,
	                             render_pass, graphics_pipeline,
	                             pipeline_layout, vertex_buffer, index_buffer,
	                             indices_size, descriptor_set, elapsed_time);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {image_available_semaphores[current_frame]};
	VkPipelineStageFlags waitStages[] = {
	    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &command_buffers[current_frame];

	VkSemaphore signalSemaphores[] = {render_finished_semaphores[imageIndex]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphics_queue, 1, &submitInfo,
	                  in_flight_fences[current_frame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swap_chain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(present_queue, &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		return true;
	if (result != VK_SUCCESS)
		throw std::runtime_error("failed to present swap chain image!");

	return false;
}
} // namespace zenithstgv
