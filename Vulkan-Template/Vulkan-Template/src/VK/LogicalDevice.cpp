//
// Created by SATAN on 12/04/2023.
//

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Synchronization/Fence.hpp"

#include <set>

LogicalDevice::LogicalDevice(LogicalDevice&& other) noexcept
	: pDevice(other.pDevice),
	device(other.device),
	graphicsQueue(std::move(other.graphicsQueue)),
	presentQueue(std::move(other.presentQueue))
{
	// Invalidate the source instance
	other.device = nullptr;
}

void LogicalDevice::commit(PhysicalDevice* pDevice)
{
	this->pDevice = pDevice;

	vk::DeviceCreateInfo deviceCreateInfo{};
	FamilyQueueIndices queueFamilyIndices = pDevice->getQueueFamilyIndices();
	std::set<uint32_t> uniqueQueueFamilies = {
			queueFamilyIndices.graphicsFamily.value(),
			queueFamilyIndices.presentFamily.value()
	};

	// Create a device queue
	float queuePriority = 1.0f;
	std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos{};
	for (auto queueFamily : uniqueQueueFamilies)
	{
		vk::DeviceQueueCreateInfo deviceQueueCreateInfo{};
		deviceQueueCreateInfo.queueFamilyIndex = queueFamily;
		deviceQueueCreateInfo.queueCount = 1;
		deviceQueueCreateInfo.pQueuePriorities = &queuePriority;
		deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
	}
	deviceCreateInfo.queueCreateInfoCount = deviceQueueCreateInfos.size();
	deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();

	vk::PhysicalDeviceFeatures deviceFeatures{};
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

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
	this->device = std::make_shared<vk::raii::Device>(*pDevice->getRaiiHandle(), deviceCreateInfo);

	this->graphicsQueue = device->getQueue(queueFamilyIndices.graphicsFamily.value(), 0);
	this->presentQueue = device->getQueue(queueFamilyIndices.presentFamily.value(), 0);
}

void LogicalDevice::submitToQueue(std::vector<vk::SubmitInfo> info, Fence* fence)
{
	graphicsQueue.submit(info, **fence->getRaiiHandle());
}

void LogicalDevice::presentInQueue(vk::PresentInfoKHR* info)
{
	if (presentQueue.presentKHR(*info) != vk::Result::eSuccess)
	{
		throw std::runtime_error("Failed to present swapchain image!");
	}
}

void LogicalDevice::waitIdle()
{
	device->waitIdle();
}
