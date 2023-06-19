#include "Surface.hpp"

#include <iostream>

namespace svk
{
	Surface::Surface(VkSurfaceKHR surface, Instance* instance) : vkHandle(surface), instance(instance){}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(instance->getVkHandle(), vkHandle, nullptr);
	}

	bool Surface::isDeviceCompatible(PhysicalDevice* device) const
	{
		SurfaceProperties properties;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->vkHandle, this->vkHandle, &properties.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device->vkHandle, this->vkHandle, &formatCount, nullptr);
		properties.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device->vkHandle, this->vkHandle, &formatCount, properties.formats.data());

		uint32_t presentCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device->vkHandle, this->vkHandle, &presentCount, nullptr);
		properties.presentModes.resize(presentCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device->vkHandle, this->vkHandle, &presentCount, properties.presentModes.data());

		// Check the device supports swapchain
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device->vkHandle, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties{extensionCount};
		vkEnumerateDeviceExtensionProperties(device->vkHandle, nullptr, &extensionCount, extensionProperties.data());

		bool foundSwapchain = false;
		for (auto& extensionProperty : extensionProperties)
		{
			if (strcmp(extensionProperty.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
			{
				// Check that the swapchain supports at least one format and one present mode
				if (properties.formats.empty() || properties.presentModes.empty())
					continue;

				foundSwapchain = true;
				break;
			}
		}

		return foundSwapchain;
	}

	void Surface::reset(VkSurfaceKHR vk_surface, Instance* instance)
	{
		this->vkHandle = vk_surface;
		this->instance = instance;
	}
}
