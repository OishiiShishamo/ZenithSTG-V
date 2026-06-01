#include "vulkan_render/command.h"

#include <stdexcept>

#include <vulkan/vulkan.hpp>

#include "main.h"
#include "vulkan_render/queue_family.h"

namespace zenithstgv {
void Command::createCommandPool(const vk::Device &device,
                                const vk::PhysicalDevice physical_device,
                                const vk::SurfaceKHR surface,
                                vk::UniqueCommandPool &command_pool) {

	const QueueFamilyIndices queueFamilyIndices =
	    findQueueFamilies(physical_device, surface);

	const vk::CommandPoolCreateInfo poolInfo{
	    vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
	    queueFamilyIndices.graphicsFamily.value()};

	command_pool = device.createCommandPoolUnique(poolInfo);
}

void Command::createCommandBuffer(
    const vk::Device &device, const vk::CommandPool command_pool,
    std::vector<vk::CommandBuffer> &command_buffers) {

	const vk::CommandBufferAllocateInfo allocInfo{
	    command_pool, vk::CommandBufferLevel::ePrimary, kMaxFramesInFlight};

	command_buffers = device.allocateCommandBuffers(allocInfo);
}

void Command::recordCommandBuffer(
    const vk::CommandBuffer command_buffer, const uint32_t image_index,
    const vk::Extent2D swap_chain_extent,
    const std::vector<vk::UniqueFramebuffer> &swap_chain_framebuffers,
    const vk::RenderPass render_pass, const vk::Pipeline graphics_pipeline,
    const vk::PipelineLayout pipeline_layout, const vk::Buffer vertex_buffer,
    const vk::Buffer index_buffer, const uint32_t indices_size,
    const vk::DescriptorSet descriptor_set, const float elapsed_time) {

	const vk::CommandBufferBeginInfo beginInfo{};

	command_buffer.begin(beginInfo);

	const vk::ClearValue clearColor{
	    std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}};

	const vk::RenderPassBeginInfo renderPassInfo{
	    render_pass,
	    swap_chain_framebuffers[image_index].get(),
	    {{{0, 0}, swap_chain_extent}},
	    1,
	    &clearColor};

	command_buffer.beginRenderPass(renderPassInfo,
	                               vk::SubpassContents::eInline);

	command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics,
	                            graphics_pipeline);

	const vk::Viewport viewport{0.0f,
	                            0.0f,
	                            static_cast<float>(swap_chain_extent.width),
	                            static_cast<float>(swap_chain_extent.height),
	                            0.0f,
	                            1.0f};

	command_buffer.setViewport(0, viewport);

	const vk::Rect2D scissor{{0, 0}, swap_chain_extent};

	command_buffer.setScissor(0, scissor);

	const vk::DeviceSize offsets[] = {0};

	command_buffer.bindVertexBuffers(0, vertex_buffer, offsets);

	command_buffer.bindIndexBuffer(index_buffer, 0, vk::IndexType::eUint16);

	command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
	                                  pipeline_layout, 0, descriptor_set,
	                                  nullptr);

	command_buffer.pushConstants<float>(pipeline_layout,
	                                    vk::ShaderStageFlagBits::eVertex |
	                                        vk::ShaderStageFlagBits::eFragment,
	                                    0, elapsed_time);

	command_buffer.drawIndexed(indices_size, 1, 0, 0, 0);

	command_buffer.endRenderPass();

	command_buffer.end();
}
} // namespace zenithstgv
