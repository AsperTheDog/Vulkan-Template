#include "GraphicsPipelineLayout.hpp"

#include "LogicalDevice.hpp"

void GraphicsPipelineLayout::commit(LogicalDevice* lDevice)
{
	// Set up the pipeline layout
	vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	pipelineLayout = std::make_shared<vk::raii::PipelineLayout>(lDevice->getRaiiHandle()->createPipelineLayout(pipelineLayoutInfo));

}
