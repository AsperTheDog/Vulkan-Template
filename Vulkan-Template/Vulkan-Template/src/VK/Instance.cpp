//
// Created by SATAN on 09/04/2023.
//

#include "Instance.hpp"

#include "../SDL/Window.hpp"
#include "PhysicalDevice.hpp"

void Instance::commit(Window* window)
{
	// Create the application info struct
	vk::ApplicationInfo appInfo{
			"Scene Viewer", VK_MAKE_VERSION(1, 0, 0),
			"No engine", VK_MAKE_VERSION(1, 0, 0),
			VK_API_VERSION_1_3
	};

	// Get the instance layers
	std::vector<vk::LayerProperties> instanceLayerProperties = context.enumerateInstanceLayerProperties();
	std::vector<char const*> instanceLayerNames;
#ifdef _DEBUG
	instanceLayerNames.push_back("VK_LAYER_KHRONOS_validation");
#endif
	if (!checkLayers(instanceLayerNames, instanceLayerProperties))
		throw std::runtime_error(
			"Set the environment variable VK_LAYER_PATH to point to the location of your layers");

	// Get the required extensions
	std::vector<const char*> requiredExtensions = window->getRequiredExtensions();
#ifdef _DEBUG
	requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

	// Create the instance
	vk::InstanceCreateInfo instanceCreateInfo({}, &appInfo, instanceLayerNames, requiredExtensions);
	this->instance = std::make_shared<vk::raii::Instance>(context, instanceCreateInfo);

	window->commitSurface(&surface, this);
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
