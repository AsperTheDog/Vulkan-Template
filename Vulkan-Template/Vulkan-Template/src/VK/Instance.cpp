//
// Created by SATAN on 09/04/2023.
//

#include "Instance.hpp"

#include "../SDL/Window.hpp"
#include "PhysicalDevice.hpp"

void mergeLists(std::vector<std::string>& from, std::vector<const char*>& to)
{
	for (auto& ext : from)
	{
		bool found = false;
		for (auto req : to)
		{
			if (strcmp(ext.c_str(), req) == 0)
				found = true;
		}
		if (!found)
			to.push_back(ext.c_str());
	}
}

void Instance::commit(Window* window)
{
	// Get the instance layers
	std::vector<vk::LayerProperties> instanceLayerProperties = context.enumerateInstanceLayerProperties();

	std::vector<char const*> instanceLayerNames;
	mergeLists(this->layers, instanceLayerNames);

	if (!checkLayers(instanceLayerNames, instanceLayerProperties))
		throw std::runtime_error("Set the environment variable VK_LAYER_PATH to point to the location of your layers");

	// Get the required extensions
	std::vector<const char*> requiredExtensions = window->getRequiredExtensions();
	mergeLists(this->extensions, requiredExtensions);

	// Create the instance
	vk::InstanceCreateInfo instanceCreateInfo({}, &appInfo, instanceLayerNames, requiredExtensions);
	this->instance = std::make_shared<vk::raii::Instance>(context, instanceCreateInfo);
}

std::vector<PhysicalDevice> Instance::getPhysicalDevices()
{
	vk::raii::PhysicalDevices physicalDevices{ *this->getRaiiHandle() };
	std::vector<PhysicalDevice> pDevices;
	for (auto& pDevice : physicalDevices)
	{
		pDevices.emplace_back(pDevice, this);
	}
	return pDevices;
}

uint32_t Instance::getVersion()
{
	return context.enumerateInstanceVersion();
}

void Instance::setAppVersion(uint32_t version)
{
	this->appInfo.apiVersion = version;
	this->version = version;
}

void Instance::setAppInfo(
	const std::string& appName, uint32_t appVersion, 
	const std::string& engineName, uint32_t engineVersion)
{
	vk::ApplicationInfo appInfo{
				appName.c_str(), appVersion,
				engineName.c_str(), engineVersion,
				version
		};
	this->appInfo = appInfo;
}


bool Instance::checkLayers(std::vector<char const*> const& layers, std::vector<vk::LayerProperties> const& properties)
{
	return std::ranges::all_of(layers,
		[&properties](char const* name)
		{
			return std::ranges::find_if(properties,
				[&name](vk::LayerProperties const& property)
				{
					return strcmp(property.layerName, name) == 0;
				}) != properties.end();
		});
}
