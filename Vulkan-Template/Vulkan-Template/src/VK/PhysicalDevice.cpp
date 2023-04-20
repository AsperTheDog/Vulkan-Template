//
// Created by SATAN on 12/04/2023.
//

#include "PhysicalDevice.hpp"
#include "Instance.hpp"

PhysicalDevice::PhysicalDevice(vk::raii::PhysicalDevice device, Instance* instance)
: instance(instance), physicalDevice(device)
{}

bool PhysicalDevice::isSuitable()
{
	auto props = this->physicalDevice.getProperties();
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = this->physicalDevice.getQueueFamilyProperties();

	// Check the device supports the API version
	if (props.apiVersion < instance->getVersion())
		return false;
	// Check the device is a discrete GPU
	if (props.deviceType != vk::PhysicalDeviceType::eDiscreteGpu)
		return false;

	// Check the device supports graphics and present
	FamilyQueueIndices indices = this->getQueueFamilyIndices();
	if (!indices.isComplete())
		return false;

	// Check the device supports swapchain
	std::vector<vk::ExtensionProperties> extensionProperties = this->physicalDevice.enumerateDeviceExtensionProperties();
	bool foundSwapchain = false;
	for (auto& extensionProperty : extensionProperties)
	{
		if (strcmp(extensionProperty.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
		{
			// Check that the swapchain supports at least one format and one present mode
			std::vector<vk::SurfaceFormatKHR> surfaceFormats = this->physicalDevice.getSurfaceFormatsKHR(instance->getSurface()->getVKHandle());
			std::vector<vk::PresentModeKHR> presentModes = this->physicalDevice.getSurfacePresentModesKHR(instance->getSurface()->getVKHandle());
			if (surfaceFormats.empty() || presentModes.empty())
				continue;

			foundSwapchain = true;
			break;
		}
	}
	if (!foundSwapchain)
		return false;
	return true;
}

FamilyQueueIndices PhysicalDevice::getQueueFamilyIndices()
{
	FamilyQueueIndices indices{};
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = this->physicalDevice.getQueueFamilyProperties();
	for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
	{
		if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
		{
			indices.graphicsFamily = i;
			break;
		}
	}
	for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
	{
		if (this->physicalDevice.getSurfaceSupportKHR(i, instance->getSurface()->getVKHandle()))
		{
			indices.presentFamily = i;
			break;
		}
	}
	return indices;
}

std::vector<vk::SurfaceFormatKHR> PhysicalDevice::getSupportedFormats()
{
	return this->physicalDevice.getSurfaceFormatsKHR(instance->getSurface()->getVKHandle());
}

std::vector<vk::PresentModeKHR> PhysicalDevice::getSupportedPresentModes()
{
	return this->physicalDevice.getSurfacePresentModesKHR(instance->getSurface()->getVKHandle());
}

vk::SurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities()
{
	return this->physicalDevice.getSurfaceCapabilitiesKHR(instance->getSurface()->getVKHandle());
}
