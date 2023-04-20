#pragma once

#include <vulkan/vulkan_raii.hpp>

class LogicalDevice;

class GraphicsPipelineLayout
{
public:
	GraphicsPipelineLayout() = default;

	void commit(LogicalDevice* lDevice);

	std::shared_ptr<vk::raii::PipelineLayout> getRaiiHandle() const { return pipelineLayout; }
	VkPipelineLayout getVKHandle() const { return **pipelineLayout; }

private:
	std::shared_ptr<vk::raii::PipelineLayout> pipelineLayout;
};

