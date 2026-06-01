#include "vulkan_render/pipeline/render_pass.h"

#include <stdexcept>

#include <vulkan/vulkan.hpp>

namespace zenithstgv {
void RenderPass::createRenderPass(const vk::Device &device,
                                  const vk::Format swap_chain_image_format,
                                  vk::UniqueRenderPass &render_pass) {
	const vk::AttachmentDescription colorAttachment{
	    {},
	    swap_chain_image_format,
	    vk::SampleCountFlagBits::e1,
	    vk::AttachmentLoadOp::eClear,
	    vk::AttachmentStoreOp::eStore,
	    vk::AttachmentLoadOp::eDontCare,
	    vk::AttachmentStoreOp::eDontCare,
	    vk::ImageLayout::eUndefined,
	    vk::ImageLayout::ePresentSrcKHR};

	const vk::AttachmentReference colorAttachmentRef{
	    0, vk::ImageLayout::eColorAttachmentOptimal};

	const vk::SubpassDescription subpass{{}, vk::PipelineBindPoint::eGraphics,
	                                     0,  nullptr,
	                                     1,  &colorAttachmentRef};

	const vk::SubpassDependency dependency{
	    VK_SUBPASS_EXTERNAL,
	    0,
	    vk::PipelineStageFlagBits::eColorAttachmentOutput,
	    vk::PipelineStageFlagBits::eColorAttachmentOutput,
	    {},
	    vk::AccessFlagBits::eColorAttachmentWrite};

	const vk::RenderPassCreateInfo renderPassInfo{
	    {}, 1, &colorAttachment, 1, &subpass, 1, &dependency};

	render_pass = device.createRenderPassUnique(renderPassInfo);
}
} // namespace zenithstgv
