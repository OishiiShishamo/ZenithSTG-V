#include "vulkan_render/draw.h"

#include <cstdint>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "vulkan_render/command.h"

namespace zenithstgv {
void Draw::drawFrame(VkDevice &device, VkSwapchainKHR swap_chain,
                     VkExtent2D swap_chain_extent,
                     std::vector<VkFramebuffer> &swap_chain_framebuffers,
                     VkRenderPass render_pass, VkPipeline graphics_pipeline,
                     VkCommandBuffer &command_buffer, VkQueue graphics_queue,
                     VkQueue present_queue,
                     VkSemaphore image_available_semaphore,
                     VkSemaphore render_finished_semaphore,
                     VkFence &in_flight_fence) {
	vkWaitForFences(device, 1, &in_flight_fence, VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &in_flight_fence);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, swap_chain, UINT64_MAX,
	                      image_available_semaphore, VK_NULL_HANDLE,
	                      &imageIndex);

	vkResetCommandBuffer(command_buffer, 0);
	Command::recordCommandBuffer(command_buffer, imageIndex, swap_chain_extent,
	                             swap_chain_framebuffers, render_pass,
	                             graphics_pipeline);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {image_available_semaphore};
	VkPipelineStageFlags waitStages[] = {
	    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &command_buffer;

	VkSemaphore signalSemaphores[] = {render_finished_semaphore};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphics_queue, 1, &submitInfo, in_flight_fence) !=
	    VK_SUCCESS) {
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

	vkQueuePresentKHR(present_queue, &presentInfo);
}
} // namespace zenithstgv
