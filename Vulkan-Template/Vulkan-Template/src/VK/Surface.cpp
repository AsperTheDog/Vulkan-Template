//
// Created by SATAN on 09/04/2023.
//

#include "Surface.hpp"
#include "Instance.hpp"
#include "PhysicalDevice.hpp"

void Surface::commit(Window* window, VkSurfaceKHR vkSurface, Instance* instance)
{
	this->window = window;
	this->instance = instance;
	this->surface = std::make_shared<vk::raii::SurfaceKHR>(*instance->getVKRaiiHandle(), vkSurface);
}

bool Surface::checkCompatibility(PhysicalDevice* pDevice)
{
	// Check the device supports swapchain
	std::vector<vk::ExtensionProperties> extensionProperties = pDevice->getVKRaiiHandle()->enumerateDeviceExtensionProperties();
	bool foundSwapchain = false;
	for (auto& extensionProperty : extensionProperties)
	{
		if (strcmp(extensionProperty.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
		{
			// Check that the swapchain supports at least one format and one present mode
			std::vector<vk::SurfaceFormatKHR> surfaceFormats = pDevice->getVKRaiiHandle()->getSurfaceFormatsKHR(getVKBaseHandle());
			std::vector<vk::PresentModeKHR> presentModes = pDevice->getVKRaiiHandle()->getSurfacePresentModesKHR(getVKBaseHandle());
			if (surfaceFormats.empty() || presentModes.empty())
				continue;

			foundSwapchain = true;
			break;
		}
	}
	return foundSwapchain;
}
