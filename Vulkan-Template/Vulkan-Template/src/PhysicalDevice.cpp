#include "PhysicalDevice.hpp"

#include "Queue.hpp"
#include "Surface.hpp"

namespace svk
{
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

	PhysicalDevice::PhysicalDevice(VkPhysicalDevice device) : vkHandle(device), id(s_idCounter)
	{
		s_idCounter++;
	}
}
