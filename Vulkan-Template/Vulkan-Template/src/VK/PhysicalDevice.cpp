//
// Created by SATAN on 12/04/2023.
//

#include "PhysicalDevice.hpp"
#include "Instance.hpp"

PhysicalDevice::PhysicalDevice(vk::raii::PhysicalDevice device, Instance* instance)
: instance(instance), physicalDevice(device)
{}

void PhysicalDevice::setQueuesToUse(RequiredFamilyQueues requiredQueues)
{
	this->requiredQueues = requiredQueues;
}

bool PhysicalDevice::isSuitable()
{
	auto props = this->physicalDevice.getProperties();
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = this->physicalDevice.getQueueFamilyProperties();

	// Check the device supports the API version
	if (props.apiVersion < instance->getVersion())
		return false;

	// Check the device supports the required familyQueues
	FamilyQueueIndices indices = this->getQueueFamilyIndices();
	if (!indices.isComplete(requiredQueues))
		return false;

	if (surface)
	{
		// Check the device supports swapchain
		std::vector<vk::ExtensionProperties> extensionProperties = this->physicalDevice.enumerateDeviceExtensionProperties();
		bool foundSwapchain = false;
		for (auto& extensionProperty : extensionProperties)
		{
			if (strcmp(extensionProperty.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
			{
				// Check that the swapchain supports at least one format and one present mode
				std::vector<vk::SurfaceFormatKHR> surfaceFormats = this->physicalDevice.getSurfaceFormatsKHR(surface->getVKHandle());
				std::vector<vk::PresentModeKHR> presentModes = this->physicalDevice.getSurfacePresentModesKHR(surface->getVKHandle());
				if (surfaceFormats.empty() || presentModes.empty())
					continue;

				foundSwapchain = true;
				break;
			}
		}
		if (!foundSwapchain)
			return false;
	}

	if (check) 
		return check(*this);

	return true;
}

FamilyQueueIndices PhysicalDevice::getQueueFamilyIndices()
{
	FamilyQueueIndices indices{};
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = this->physicalDevice.getQueueFamilyProperties();
	if (requiredQueues.present)
	{
		if (!surface) 
			throw std::runtime_error("Requested present family but Surface is null");
		for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
		{
			if (this->physicalDevice.getSurfaceSupportKHR(i, surface->getVKHandle()))
			{
				indices.presentFamily = i;
				break;
			}
		}
	}
	if (!requiredQueues.graphics && !requiredQueues.compute)
		return indices;

	for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
	{
		if (requiredQueues.graphics && queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
			indices.graphicsFamily = i;
		
		if (requiredQueues.compute && queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eCompute)
			indices.computeFamily = i;

		if (indices.isComplete(requiredQueues))
			break;
	}
	return indices;
}

FamilyQueueIndices PhysicalDevice::getQueueFamilyIndices(RequiredFamilyQueues requiredTypes)
{
	auto temp = this->requiredQueues;
	this->requiredQueues = requiredTypes;
	auto indices = this->getQueueFamilyIndices();
	this->requiredQueues = temp;
	return indices;
}

std::vector<vk::SurfaceFormatKHR> PhysicalDevice::getSupportedFormats()
{
	return this->physicalDevice.getSurfaceFormatsKHR(surface->getVKHandle());
}

std::vector<vk::PresentModeKHR> PhysicalDevice::getSupportedPresentModes()
{
	return this->physicalDevice.getSurfacePresentModesKHR(surface->getVKHandle());
}

vk::SurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities()
{
	return this->physicalDevice.getSurfaceCapabilitiesKHR(surface->getVKHandle());
}
