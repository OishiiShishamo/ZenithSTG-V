#include "vulkan_render/pipeline/graphics_pipeline.h"

#include <array>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "utility/read_file.h"
#include "vertex.h"

namespace zenithstgv {
void GraphicsPipeline::createGraphicsPipeline(
    vk::Device device, vk::RenderPass render_pass,
    vk::DescriptorSetLayout descriptor_set_layout,
    vk::UniquePipelineLayout &pipeline_layout,
    vk::UniquePipeline &graphics_pipeline) {
	const auto vertShaderCode = Utility::ReadFile("shaders/basic_vert.spv");

	const auto fragShaderCode = Utility::ReadFile("shaders/basic_frag.spv");

	const auto vertShaderModule = createShaderModule(vertShaderCode, device);

	const auto fragShaderModule = createShaderModule(fragShaderCode, device);

	const vk::PipelineShaderStageCreateInfo vertShaderStageInfo{
	    {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule.get(), "main"};

	const vk::PipelineShaderStageCreateInfo fragShaderStageInfo{
	    {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule.get(), "main"};

	const std::array shaderStages{vertShaderStageInfo, fragShaderStageInfo};

	const auto bindingDescription = Vertex::getBindingDescription();

	const auto attributeDescriptions = Vertex::getAttributeDescriptions();

	const vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
	    {},
	    1,
	    &bindingDescription,
	    static_cast<uint32_t>(attributeDescriptions.size()),
	    attributeDescriptions.data()};

	const vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
	    {}, vk::PrimitiveTopology::eTriangleList, vk::False};

	const vk::PipelineViewportStateCreateInfo viewportState{
	    {}, 1, nullptr, 1, nullptr};

	const vk::PipelineRasterizationStateCreateInfo rasterizer{
	    {},
	    vk::False,
	    vk::False,
	    vk::PolygonMode::eFill,
	    vk::CullModeFlagBits::eBack,
	    vk::FrontFace::eClockwise,
	    vk::False,
	    0.0f,
	    0.0f,
	    0.0f,
	    1.0f};

	const vk::PipelineMultisampleStateCreateInfo multisampling{
	    {}, vk::SampleCountFlagBits::e1, vk::False};

	const vk::PipelineColorBlendAttachmentState colorBlendAttachment{
	    vk::False,
	    {},
	    {},
	    {},
	    {},
	    {},
	    {},
	    vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
	        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};

	const vk::PipelineColorBlendStateCreateInfo colorBlending{
	    {}, vk::False, vk::LogicOp::eCopy, 1, &colorBlendAttachment};

	const std::array dynamicStates{vk::DynamicState::eViewport,
	                               vk::DynamicState::eScissor};

	const vk::PipelineDynamicStateCreateInfo dynamicState{
	    {}, static_cast<uint32_t>(dynamicStates.size()), dynamicStates.data()};

	const vk::PushConstantRange pushConstantRange{
	    vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
	    0, sizeof(float)};

	const vk::PipelineLayoutCreateInfo pipelineLayoutInfo{
	    {}, 1, &descriptor_set_layout, 1, &pushConstantRange};

	pipeline_layout = device.createPipelineLayoutUnique(pipelineLayoutInfo);

	const vk::GraphicsPipelineCreateInfo pipelineInfo{
	    {},
	    static_cast<uint32_t>(shaderStages.size()),
	    shaderStages.data(),
	    &vertexInputInfo,
	    &inputAssembly,
	    nullptr,
	    &viewportState,
	    &rasterizer,
	    &multisampling,
	    nullptr,
	    &colorBlending,
	    &dynamicState,
	    pipeline_layout.get(),
	    render_pass,
	    0};

	graphics_pipeline = std::move(
	    device.createGraphicsPipelineUnique(nullptr, pipelineInfo).value);
}

vk::UniqueShaderModule
GraphicsPipeline::createShaderModule(const std::vector<char> &code,
                                     vk::Device device) {
	return device.createShaderModuleUnique(vk::ShaderModuleCreateInfo{
	    {}, code.size(), reinterpret_cast<const uint32_t *>(code.data())});
}
} // namespace zenithstgv
