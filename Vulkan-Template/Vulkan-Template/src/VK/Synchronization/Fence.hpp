#pragma once


#include <vulkan/vulkan_raii.hpp>

class LogicalDevice;

class Fence
{
public:
	Fence() = default;

	void commit(LogicalDevice* lDevice);

	std::shared_ptr<vk::raii::Fence> getVKRaiiHandle() const { return fence; }
	VkFence getVKBaseHandle() const { return **fence; }
	void wait();
	void reset();

private:
	std::shared_ptr<vk::raii::Fence> fence;
	LogicalDevice* logicalDevice;
};

