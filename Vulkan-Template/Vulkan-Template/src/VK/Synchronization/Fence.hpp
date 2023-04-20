#pragma once


#include <vulkan/vulkan_raii.hpp>

class LogicalDevice;

class Fence
{
public:
	Fence() = default;

	void commit(LogicalDevice* lDevice);

	std::shared_ptr<vk::raii::Fence> getRaiiHandle() const { return fence; }
	VkFence getVKHandle() const { return **fence; }
	void wait();
	void reset();

private:
	std::shared_ptr<vk::raii::Fence> fence;
	LogicalDevice* lDevice;
};

