#include "Instance.hpp"

#include "PhysicalDevice.hpp"

#include <stdexcept>

namespace svk
{
	Instance::~Instance()
	{
		vkDestroyInstance(this->vkHandle, nullptr);
	}

	void Instance::commit(uint32_t apiVersion)
	{
		this->apiVersion = apiVersion;
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.apiVersion = apiVersion;
		appInfo.applicationVersion = this->appVersion;
		appInfo.pApplicationName = this->appName.c_str();
		appInfo.engineVersion = this->engineVersion;
		appInfo.pEngineName = this->engineName.c_str();

		VkInstanceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		info.pApplicationInfo = &appInfo;
		if (!this->extensions.empty())
		{
			info.enabledExtensionCount = static_cast<uint32_t>(this->extensions.size());
			info.ppEnabledExtensionNames = this->extensions.data();
		}
		if (!this->layers.empty())
		{
			info.enabledLayerCount = static_cast<uint32_t>(this->layers.size());
			info.ppEnabledLayerNames = this->layers.data();
		}

		if (vkCreateInstance(&info, nullptr, &this->vkHandle) != VK_SUCCESS)
			throw std::runtime_error("Error while creating instance!");

		uint32_t count = 0;
		vkEnumeratePhysicalDevices(this->vkHandle, &count, nullptr);
		std::vector<VkPhysicalDevice> devices(count);
		vkEnumeratePhysicalDevices(this->vkHandle, &count, devices.data());

		this->physicalDevices.reserve(count);
		for (auto& vkDevice : devices)
		{
			PhysicalDevice pDevice{ vkDevice };
			this->physicalDevices.push_back(pDevice);
		}
	}

	void Instance::addValidationLayers()
	{
		this->addLayer("VK_LAYER_KHRONOS_validation");
		this->addExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	void Instance::addLayer(const char* layer)
	{
		for (auto& existing : this->layers)
			if (existing == layer) return;
		this->layers.push_back(layer);
	}

	void Instance::addExtension(const char* extension)
	{
		for (auto& existing : this->extensions)
			if (existing == extension) return;
		this->extensions.push_back(extension);
	}

	bool Instance::areAddedExtensionsSupported()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
		
		std::vector<const char*> tmpExtensions(this->extensions.begin(), this->extensions.end());

		for (const auto& availableExtension : availableExtensions) {
			for (const auto& addedExtension : this->extensions)
			{
				if (strcmp(addedExtension, availableExtension.extensionName) == 0)
				{
					tmpExtensions.erase(std::ranges::find(tmpExtensions, addedExtension));
				}
			}
		}
		return tmpExtensions.empty();
	}

	std::vector<PhysicalDevice>* Instance::getPhysicalDevices()
	{
		return &physicalDevices;
	}
}
