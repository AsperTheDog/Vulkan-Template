#include "Surface.hpp"

namespace svk
{
	Surface::Surface(VkSurfaceKHR surface, Instance* instance) : vkHandle(surface), instance(instance){}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(instance->getVkHandle(), vkHandle, nullptr);
	}
	
	SurfaceProperties Surface::getProperties(const PhysicalDevice* device) const
	{
		SurfaceProperties properties;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->getVKHandle(), vkHandle, &properties.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device->getVKHandle(), vkHandle, &formatCount, nullptr);
		if (formatCount != 0)
		{
			properties.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device->getVKHandle(), vkHandle, &formatCount, properties.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device->getVKHandle(), vkHandle, &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			properties.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device->getVKHandle(), vkHandle, &presentModeCount, properties.presentModes.data());
		}
		
		return properties;
	}

	void Surface::reset(VkSurfaceKHR vk_surface, Instance* instance)
	{
		this->vkHandle = vk_surface;
		this->instance = instance;
	}
}
