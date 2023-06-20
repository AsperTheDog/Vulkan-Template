#include "PhysicalDevice.hpp"

#include <set>

#include "Queue.hpp"
#include "Surface.hpp"

namespace svk
{
	bool PhysicalDevice::areExtensionsSupported(std::vector<const char*> exts)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(vkHandle, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(vkHandle, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(exts.begin(), exts.end());

		for (const auto& extension : availableExtensions)
			requiredExtensions.erase(extension.extensionName);

		return requiredExtensions.empty();
	}

	bool PhysicalDevice::isExtensionSupported(const char* ext)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(vkHandle, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(vkHandle, nullptr, &extensionCount, availableExtensions.data());

		for (const auto& extension : availableExtensions)
			if (strcmp(extension.extensionName, ext) == 0)
				return true;
		return false;
	}

	bool PhysicalDevice::isFormatSupported(const Surface& surface, VkSurfaceFormatKHR format)
	{
		auto surfaceProperties = surface.getProperties(this);
		for (auto& availableFormat : surfaceProperties.formats)
		{
			if (availableFormat.format == format.format && availableFormat.colorSpace == format.colorSpace)
				return true;
		}
		return false;
	}

	bool PhysicalDevice::isPresentModeSupported(const Surface& surface, VkPresentModeKHR presentMode)
	{
		auto surfaceProperties = surface.getProperties(this);
		for (auto& availableMode : surfaceProperties.presentModes)
		{
			if (availableMode == presentMode)
				return true;
		}
		return false;
	}

	VkPhysicalDeviceProperties PhysicalDevice::getProperties() const
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(vkHandle, &properties);
		return properties;
	}

	VkPhysicalDeviceFeatures PhysicalDevice::getFeatures() const
	{
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(vkHandle, &features);
		return features;
	}

	VkPhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties() const
	{
		VkPhysicalDeviceMemoryProperties properties;
		vkGetPhysicalDeviceMemoryProperties(vkHandle, &properties);
		return properties;
	}

	std::vector<QueueFamily> PhysicalDevice::getQueueFamilies() const
	{
		uint32_t familyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(vkHandle, &familyCount, nullptr);
		std::vector<VkQueueFamilyProperties> vkFamilies{familyCount};
		vkGetPhysicalDeviceQueueFamilyProperties(vkHandle, &familyCount, vkFamilies.data());

		std::vector<QueueFamily> families;
		for (uint32_t i = 0; i < familyCount; i++)
		{
			QueueFamily fam(this, vkFamilies[i], i);
			families.push_back(fam);
		}
		return families;
	}

	SurfaceProperties PhysicalDevice::getSurfaceProperties(const Surface& surface) const
	{
		return surface.getProperties(this);
	}

	PhysicalDevice::PhysicalDevice(VkPhysicalDevice device) : vkHandle(device), id(s_idCounter)
	{
		s_idCounter++;
	}
}
