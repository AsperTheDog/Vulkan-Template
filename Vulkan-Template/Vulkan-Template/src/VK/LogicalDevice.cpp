//
// Created by SATAN on 12/04/2023.
//

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Synchronization/Fence.hpp"

void LogicalDevice::commit()
{
	// Create a device vkQueue
	std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos{};
	auto queueFamilies = physicalDevice->queues.getQueues();
	for (QueueFamilies::QueueHolder& queueFamily : *queueFamilies)
	{
		vk::DeviceQueueCreateInfo deviceQueueCreateInfo{};
		deviceQueueCreateInfo.queueFamilyIndex = queueFamily.queue.index;
		deviceQueueCreateInfo.queueCount = queueFamily.queue.count;
		deviceQueueCreateInfo.pQueuePriorities = queueFamily.priorities.data();
		deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
	}

	vk::DeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.queueCreateInfoCount = deviceQueueCreateInfos.size();
	deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
	
	deviceCreateInfo.pEnabledFeatures = &features;

	// Enable the swapchain extension
	deviceCreateInfo.enabledExtensionCount = 1;
	const char* extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	deviceCreateInfo.ppEnabledExtensionNames = extensions;

#ifdef _DEBUG
	const char* extensionLayers[] = { "VK_LAYER_KHRONOS_validation" };
	deviceCreateInfo.enabledLayerCount = 1;
	deviceCreateInfo.ppEnabledLayerNames = extensionLayers;
#else
	deviceCreateInfo.enabledLayerCount = 0;
#endif

	// Create the device
	this->device = std::make_shared<vk::raii::Device>(*physicalDevice->getVKRaiiHandle(), deviceCreateInfo);

	physicalDevice->queues.commitQueues(this);
}

void LogicalDevice::setQueuePriorities(uint32_t queueIndex, const std::vector<float>& priorities)
{
	physicalDevice->queues.updateQueue(queueIndex, priorities);
}

void LogicalDevice::submitToQueue(
	FamilyQueueType type, uint32_t offset, 
	const std::vector<vk::SubmitInfo>& info, 
	Fence* fence)
{
	vk::raii::Queue* queue = nullptr;
	uint32_t count = 0;
	for (auto& lQueue : physicalDevice->queues.queues)
	{
		if (lQueue.queue.type & type)
		{
			if (count == offset)
			{
				queue = &lQueue.queue.vkQueue;
				break;
			}
			count++;
		}
	}
#ifdef _DEBUG
	if (queue == nullptr)
	{
		throw std::runtime_error("Failed to find queue on submit");
	}
#endif

	queue->submit(info, **fence->getVKRaiiHandle());
}

void LogicalDevice::presentInQueue(uint32_t offset, vk::PresentInfoKHR* info)
{
	vk::raii::Queue* queue = nullptr;
	uint32_t count = 0;
	for (auto& lQueue : physicalDevice->queues.queues)
	{
		if (lQueue.queue.type & FamilyQueuePresent)
		{
			if (count == offset)
			{
				queue = &lQueue.queue.vkQueue;
				break;
			}
			count++;
		}
	}
#ifdef _DEBUG
	if (queue == nullptr)
	{
		throw std::runtime_error("Failed to find queue on present");
	}
	if (queue->presentKHR(*info) != vk::Result::eSuccess)
	{
		throw std::runtime_error("Failed to present swapchain image!");
	}
#endif
	queue->presentKHR(*info);
}

void LogicalDevice::waitIdle()
{
	device->waitIdle();
}

void LogicalDevice::setQueue(vk::raii::Queue& queue, uint32_t index, int i)
{
	queue = device->getQueue(index, i);
}
