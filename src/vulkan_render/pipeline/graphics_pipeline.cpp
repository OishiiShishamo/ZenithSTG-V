#include "vulkan_render/pipeline/graphics_pipeline.h"

#include <array>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "instance_data.h"
#include "utility/read_file.h"
#include "vertex.h"

namespace zenithstgv {
void GraphicsPipeline::createGraphicsPipelines(
    vk::Device device, vk::RenderPass render_pass,
    vk::DescriptorSetLayout descriptor_set_layout,
    vk::UniquePipelineLayout &pipeline_layout,
    std::array<vk::UniquePipeline, 4> &pipelines) {

	const auto vertShaderCode = Utility::ReadFile("shaders/basic_vert.spv");
	const auto fragShaderCode = Utility::ReadFile("shaders/basic_frag.spv");
	const auto vertShaderModule = createShaderModule(vertShaderCode, device);
	const auto fragShaderModule = createShaderModule(fragShaderCode, device);

	const vk::PipelineShaderStageCreateInfo vertShaderStageInfo{
	    {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule.get(), "main"};
	const vk::PipelineShaderStageCreateInfo fragShaderStageInfo{
	    {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule.get(), "main"};
	const std::array shaderStages{vertShaderStageInfo, fragShaderStageInfo};

	const auto vertexBindingDesc = Vertex::getBindingDescription();
	const auto instanceBindingDesc = InstanceData::getBindingDescription();
	const std::array bindingDescs{vertexBindingDesc, instanceBindingDesc};

	const auto vertexAttrDescs = Vertex::getAttributeDescriptions();
	const auto instanceAttrDescs = InstanceData::getAttributeDescriptions();
	std::vector<vk::VertexInputAttributeDescription> allAttrDescs;

	for (const auto &a : vertexAttrDescs)
		allAttrDescs.push_back(a);
	for (const auto &a : instanceAttrDescs)
		allAttrDescs.push_back(a);

	const vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
	    {},
	    static_cast<uint32_t>(bindingDescs.size()),
	    bindingDescs.data(),
	    static_cast<uint32_t>(allAttrDescs.size()),
	    allAttrDescs.data()};

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
	    vk::FrontFace::eCounterClockwise,
	    vk::False,
	    0.0f,
	    0.0f,
	    0.0f,
	    1.0f};
	const vk::PipelineMultisampleStateCreateInfo multisampling{
	    {}, vk::SampleCountFlagBits::e1, vk::False};
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

	const std::array<vk::PipelineColorBlendAttachmentState, 4>
	    blendAttachments = {{
	        // Normal
	        {vk::True, vk::BlendFactor::eSrcAlpha,
	         vk::BlendFactor::eOneMinusSrcAlpha, vk::BlendOp::eAdd,
	         vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
	         vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
	             vk::ColorComponentFlagBits::eB |
	             vk::ColorComponentFlagBits::eA},
	        // Add
	        {vk::True, vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOne,
	         vk::BlendOp::eAdd, vk::BlendFactor::eOne, vk::BlendFactor::eZero,
	         vk::BlendOp::eAdd,
	         vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
	             vk::ColorComponentFlagBits::eB |
	             vk::ColorComponentFlagBits::eA},
	        // Subtract
	        {vk::True, vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOne,
	         vk::BlendOp::eReverseSubtract, vk::BlendFactor::eOne,
	         vk::BlendFactor::eZero, vk::BlendOp::eAdd,
	         vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
	             vk::ColorComponentFlagBits::eB |
	             vk::ColorComponentFlagBits::eA},
	        // Multiply
	        {vk::True, vk::BlendFactor::eDstColor, vk::BlendFactor::eZero,
	         vk::BlendOp::eAdd, vk::BlendFactor::eOne, vk::BlendFactor::eZero,
	         vk::BlendOp::eAdd,
	         vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
	             vk::ColorComponentFlagBits::eB |
	             vk::ColorComponentFlagBits::eA},
	    }};

	for (int i = 0; i < 4; i++) {
		const vk::PipelineColorBlendStateCreateInfo colorBlending{
		    {}, vk::False, vk::LogicOp::eCopy, 1, &blendAttachments[i]};

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

		pipelines[i] = std::move(
		    device.createGraphicsPipelineUnique(nullptr, pipelineInfo).value);
	}
}

vk::UniqueShaderModule
GraphicsPipeline::createShaderModule(const std::vector<char> &code,
                                     vk::Device device) {
	return device.createShaderModuleUnique(vk::ShaderModuleCreateInfo{
	    {}, code.size(), reinterpret_cast<const uint32_t *>(code.data())});
}
} // namespace zenithstgv
