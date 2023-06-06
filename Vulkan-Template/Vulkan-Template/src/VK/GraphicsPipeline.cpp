//
// Created by SATAN on 15/04/2023.
//

#include <iostream>
#include <fstream>

#include "GraphicsPipeline.hpp"

#include "GraphicsPipelineLayout.hpp"
#include "LogicalDevice.hpp"
#include "Swapchain.hpp"

void GraphicsPipeline::commit(Swapchain* swapchain)
{
	this->swapchain = swapchain;

	std::string fragSource;
	std::string vertSource;
	std::ifstream fragFile("shaders/shader.frag");
	std::ifstream vertFile("shaders/shader.vert");

	// Check that the files have been opened
	if (!fragFile.is_open() || !vertFile.is_open())
	{
		std::cerr << "Could not open shader files" << std::endl;
		return;
	}

	std::getline(fragFile, fragSource, '\0');
	std::getline(vertFile, vertSource, '\0');

	auto fragSpirv = compile_file("shader.frag", shaderc_shader_kind::shaderc_fragment_shader, fragSource, true);
	auto vertSpirv = compile_file("shader.vert", shaderc_shader_kind::shaderc_vertex_shader, vertSource, true);

	// Create the shader modules
	vk::ShaderModuleCreateInfo fragModuleInfo({}, fragSpirv.size() * sizeof(uint32_t), fragSpirv.data());
	vk::ShaderModuleCreateInfo vertModuleInfo({}, vertSpirv.size() * sizeof(uint32_t), vertSpirv.data());

	auto fragModule = swapchain->getLogicalDevice()->getVKRaiiHandle()->createShaderModule(fragModuleInfo);
	auto vertModule = swapchain->getLogicalDevice()->getVKRaiiHandle()->createShaderModule(vertModuleInfo);

	// Create the shader stages
	vk::PipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
	fragShaderStageInfo.module = *fragModule;
	fragShaderStageInfo.pName = "main";

	vk::PipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
	vertShaderStageInfo.module = *vertModule;
	vertShaderStageInfo.pName = "main";

	std::array shaderStages = { fragShaderStageInfo, vertShaderStageInfo };

	// Set up the dynamic state
	std::array dynamicStates = {
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor
	};

	vk::PipelineDynamicStateCreateInfo dynamicStateInfo{};
	dynamicStateInfo.dynamicStateCount = dynamicStates.size();
	dynamicStateInfo.pDynamicStates = dynamicStates.data();

	// Set up the vertex input
	vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;

	// Set up the input assembly
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
	inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	// Set up the viewport
	vk::Viewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapchain->getExtent().width;
	viewport.height = (float) swapchain->getExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	vk::Rect2D scissor{};
	scissor.offset = vk::Offset2D(0, 0);
	scissor.extent = swapchain->getExtent();

	vk::PipelineViewportStateCreateInfo viewportStateInfo{};
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.pScissors = &scissor;

	// Set up the rasterizer
	vk::PipelineRasterizationStateCreateInfo rasterizerInfo{};
	rasterizerInfo.depthClampEnable = VK_FALSE;
	rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizerInfo.polygonMode = vk::PolygonMode::eFill;
	rasterizerInfo.lineWidth = 1.0f;
	rasterizerInfo.cullMode = vk::CullModeFlagBits::eBack;
	rasterizerInfo.frontFace = vk::FrontFace::eClockwise;
	rasterizerInfo.depthBiasEnable = VK_FALSE;
	rasterizerInfo.depthBiasConstantFactor = 0.0f;
	rasterizerInfo.depthBiasClamp = 0.0f;
	rasterizerInfo.depthBiasSlopeFactor = 0.0f;

	// Set up the multisampling
	vk::PipelineMultisampleStateCreateInfo multisamplingInfo{};
	multisamplingInfo.sampleShadingEnable = VK_FALSE;
	multisamplingInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;
	multisamplingInfo.minSampleShading = 1.0f;
	multisamplingInfo.pSampleMask = nullptr;
	multisamplingInfo.alphaToCoverageEnable = VK_FALSE;
	multisamplingInfo.alphaToOneEnable = VK_FALSE;

	// Set up the color blending
	vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne;
	colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eZero;
	colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
	colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
	colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
	colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;

	vk::PipelineColorBlendStateCreateInfo colorBlendingInfo{};
	colorBlendingInfo.logicOpEnable = VK_FALSE;
	colorBlendingInfo.logicOp = vk::LogicOp::eCopy;
	colorBlendingInfo.attachmentCount = 1;
	colorBlendingInfo.pAttachments = &colorBlendAttachment;
	colorBlendingInfo.blendConstants[0] = 0.0f;
	colorBlendingInfo.blendConstants[1] = 0.0f;
	colorBlendingInfo.blendConstants[2] = 0.0f;
	colorBlendingInfo.blendConstants[3] = 0.0f;

    // Configure color attachment
    vk::AttachmentDescription colorAttachment{};
    colorAttachment.format = swapchain->getPresentProperties().format;
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    // Configure subpass
    vk::AttachmentReference colorAttachmentRef;
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass{};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

	// Commit
	renderPass.addAttachment(colorAttachment);
	renderPass.addSubPass(subpass);
	renderPass.commit(this, swapchain);

	graphicsPipelineLayout.commit(swapchain->getLogicalDevice());

	// Create the graphics pipeline
	vk::GraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportStateInfo;
	pipelineInfo.pRasterizationState = &rasterizerInfo;
	pipelineInfo.pMultisampleState = &multisamplingInfo;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState = &colorBlendingInfo;
	pipelineInfo.pDynamicState = &dynamicStateInfo;
	pipelineInfo.layout = graphicsPipelineLayout.getVKHandle();
	pipelineInfo.renderPass = renderPass.getVKHandle();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = nullptr;
	pipelineInfo.basePipelineIndex = -1;

	graphicsPipeline = std::make_shared<vk::raii::Pipeline>(swapchain->getLogicalDevice()->getVKRaiiHandle()->createGraphicsPipeline(nullptr, pipelineInfo));

	swapchain->generateFBs(&renderPass);
}

RenderPass* GraphicsPipeline::getRenderPass()
{
	return &renderPass;
}

// Compiles a shader to a SPIR-V binary. Returns the binary as
// a vector of 32-bit words.
std::vector<uint32_t> GraphicsPipeline::compile_file(const std::string& source_name,
	shaderc_shader_kind kind,
	const std::string& source,
	bool optimize)
{
	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	
	if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_performance);

	shaderc::SpvCompilationResult module =
		compiler.CompileGlslToSpv(source, kind, source_name.c_str(), options);

	if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
		std::cerr << module.GetErrorMessage();
		return {};
	}

	return { module.cbegin(), module.cend() };
}