#pragma once


#include <vulkan/vulkan_raii.hpp>

class LogicalDevice;

class Semaphore
{
public:
	Semaphore() = default;

	void commit(LogicalDevice* lDevice);

	std::shared_ptr<vk::raii::Semaphore> getVKRaiiHandle() const { return semaphore; }
	VkSemaphore getVKBaseHandle() const { return **semaphore; }

private:
	std::shared_ptr<vk::raii::Semaphore> semaphore;
};

