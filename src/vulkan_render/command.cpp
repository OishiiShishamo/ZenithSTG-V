#include "vulkan_render/command.h"

#include <stdexcept>

#include <vulkan/vulkan.h>

#include "main.h"
#include "vulkan_render/queue_family.h"

namespace zenithstgv {
void Command::createCommandPool(VkDevice device,
                                VkPhysicalDevice physical_device,
                                VkSurfaceKHR surface,
                                VkCommandPool &command_pool) {
	QueueFamilyIndices queueFamilyIndices =
	    findQueueFamilies(physical_device, surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &command_pool) !=
	    VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void Command::createCommandBuffer(
    VkDevice device, VkCommandPool command_pool,
    std::vector<VkCommandBuffer> &command_buffers) {
	command_buffers.resize(kMaxFramesInFlight);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = command_pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount =
	    static_cast<uint32_t>(command_buffers.size());

	if (vkAllocateCommandBuffers(device, &allocInfo, command_buffers.data()) !=
	    VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void Command::recordCommandBuffer(
    VkCommandBuffer commandBuffer, uint32_t imageIndex,
    VkExtent2D swap_chain_extent,
    std::vector<VkFramebuffer> &swap_chain_framebuffers,
    VkRenderPass render_pass, VkPipeline graphics_pipeline,
    VkPipelineLayout pipeline_layout, VkBuffer vertex_buffer,
    VkBuffer index_buffer, uint32_t indices_size,
    VkDescriptorSet descriptor_set, float elapsed_time) {
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = render_pass;
	renderPassInfo.framebuffer = swap_chain_framebuffers[imageIndex];
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swap_chain_extent;

	VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
	                     VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
	                  graphics_pipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swap_chain_extent.width);
	viewport.height = static_cast<float>(swap_chain_extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swap_chain_extent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	VkBuffer vertexBuffers[] = {vertex_buffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer, index_buffer, 0, VK_INDEX_TYPE_UINT16);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
	                        pipeline_layout, 0, 1, &descriptor_set, 0, nullptr);
	vkCmdPushConstants(commandBuffer, pipeline_layout,
	                   VK_SHADER_STAGE_VERTEX_BIT |
	                       VK_SHADER_STAGE_FRAGMENT_BIT,
	                   0, sizeof(float), &elapsed_time);
	vkCmdDrawIndexed(commandBuffer, indices_size, 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}
} // namespace zenithstgv
