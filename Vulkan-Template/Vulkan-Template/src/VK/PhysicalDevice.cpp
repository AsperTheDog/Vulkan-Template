//
// Created by SATAN on 12/04/2023.
//

#include "PhysicalDevice.hpp"

#include "QueueFamilies.hpp"
#include "Instance.hpp"

PhysicalDevice::PhysicalDevice(vk::raii::PhysicalDevice device, Instance* instance)
	: queues(), instance(instance), vkraiiHandle(std::move(device))
{
}

std::vector<vk::SurfaceFormatKHR> PhysicalDevice::getSupportedFormats()
{
	return this->vkraiiHandle.getSurfaceFormatsKHR(surface->getVKBaseHandle());
}

std::vector<vk::PresentModeKHR> PhysicalDevice::getSupportedPresentModes()
{
	return this->vkraiiHandle.getSurfacePresentModesKHR(surface->getVKBaseHandle());
}

vk::SurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities()
{
	return this->vkraiiHandle.getSurfaceCapabilitiesKHR(surface->getVKBaseHandle());
}

uint32_t PhysicalDevice::getApiVersion()
{
	return this->vkraiiHandle.getProperties().apiVersion;
}