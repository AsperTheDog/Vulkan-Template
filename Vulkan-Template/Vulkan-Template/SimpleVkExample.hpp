#pragma once
#include <algorithm>
#include <iostream>

#include "SimpleVk.hpp"
#include "Window.hpp"

class SimpleVkExample
{
public:

	SimpleVkExample() : window(1920, 1080, "SimpleVk Example")
	{
		std::cout << "Initiated window\n\n";
		this->initInstance();
		std::cout << "\n";
		this->createSurface();
		std::cout << "\n";
		this->getPhysicalDevice();
		std::cout << "\n";
		this->getLogicalDevice();
		std::cout << "\n";
		this->getSwapchain();
	}

private:
	svk::Instance instance;
	svk::Window window;
	svk::PhysicalDevice* physicalDevice{};
	svk::LogicalDevice logicalDevice;

	svk::QueuePosition graphicsQueue{};
	svk::QueuePosition presentQueue{};
	svk::QueuePosition computeQueue{};

	std::vector<const char*> requiredExts = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	void initInstance()
	{
#ifdef _DEBUG
		instance.addValidationLayers();
#endif

		auto requiredExts = window.getRequiredExtensions();
		for (auto ext : requiredExts)
		{
			instance.addExtension(ext);
			std::cout << " - Added instance extension: " << ext << "\n";
		}

		if (!instance.areAddedExtensionsSupported())
		{
			throw std::runtime_error("Required extensions are not supported");
		}
		std::cout << "All required extensions are supported\n";

		instance.setAppName("SimpleVk Example");
		instance.commit(svk::V_1_3);

		std::cout << "Created instance (using version " << svk::V_1_3 << ")\n";
	}

	void createSurface()
	{
		window.createSurface(&instance);
		std::cout << "Created surface\n";
	}

	void getPhysicalDevice()
	{
		auto devices = instance.getPhysicalDevices();
		for (auto& device : *devices)
		{
			std::cout << "Checking compatibility with device: " << device.getProperties().deviceName << "\n";
			auto properties = device.getProperties();

			if (properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				std::cout << " - UNSUITABLE: Device is not a discrete GPU\n";
				continue;
			}
			std::cout << " - Device is a discrete GPU\n";

			if (properties.apiVersion < instance.getApiVersion())
			{
				std::cout << " - UNSUITABLE: Device does not support Vulkan " << instance.getApiVersion() << "\n";
				continue;
			}
			std::cout << " - Device supports Vulkan " << instance.getApiVersion() << "\n";

			if (!window.getSurface().isDeviceCompatible(&device))
			{
				std::cout << " - UNSUITABLE: Device is not compatible with the current surface\n";
				continue;
			}
			std::cout << " - Device is compatible with the current surface\n";

			bool types[4];
			types[svk::QueueFamilyTypes::GRAPHICS] = true;
			types[svk::QueueFamilyTypes::COMPUTE] = true;
			types[svk::QueueFamilyTypes::TRANSFER] = false;
			types[svk::QueueFamilyTypes::BINDING] = false;
			if (!doesDeviceSupportQueueFamilyTypes(&device, types))
			{
				std::cout << " - UNSUITABLE: Device does not support required queue family types\n";
				continue;
			}
			std::cout << " - Device supports required queue family types\n";

			if (!doesDeviceContainPresentQueue(&device))
			{
				std::cout << " - UNSUITABLE: Device does not contain a queue family that supports presentation\n";
				continue;
			}
			std::cout << " - Device contains a queue family that supports presentation\n";

			if (!device.areExtensionsSupported(requiredExts))
			{
				std::cout << " - UNSUITABLE: Device does not support the necessary extensions\n";
				continue;
			}
			std::cout << " - Device supports all necessary extensions\n";

			physicalDevice = &device;
			break;
		}

		auto surfaceProperties = physicalDevice->getSurfaceProperties(window.getSurface());
		// Print all properties
		std::cout << "\nSurface properties:\n";
		for (auto prop : surfaceProperties.formats)
		{
			std::cout << " - Format: " << std::to_string(prop.format) << ", colorSpace: " << std::to_string(prop.colorSpace) << "\n";
		}
		for (auto prop : surfaceProperties.presentModes)
		{
			std::cout << " - Present mode: " << std::to_string(prop) << "\n";
		}

		std::cout << "Device is suitable, selected for use\n";
	}

	void getLogicalDevice()
	{
#ifdef _DEBUG
		logicalDevice.addValidationLayers();
#endif

		for (auto& ext : requiredExts)
			logicalDevice.addExtension(ext);

		auto queues = physicalDevice->getQueueFamilies();
		svk::QueueFamily* graphicsQueueFamily = nullptr;
		svk::QueueFamily* presentQueueFamily = nullptr;
		svk::QueueFamily* computeQueueFamily = nullptr;
		for (auto& queue : queues)
		{
			if (queue.types[svk::QueueFamilyTypes::GRAPHICS] && graphicsQueueFamily == nullptr)
			{
				std::cout << "Found graphics queue family with index " << queue.index << "\n";
				graphicsQueueFamily = &queue;
			}
			if (queue.supportsPresent(&window.getSurface()) && presentQueueFamily == nullptr)
			{
				std::cout << "Found present queue family with index " << queue.index << "\n";
				presentQueueFamily = &queue;
			}
			if (queue.types[svk::QueueFamilyTypes::COMPUTE] && computeQueueFamily == nullptr)
			{
				std::cout << "Found compute queue family with index " << queue.index << "\n";
				computeQueueFamily = &queue;
			}
		}

		graphicsQueue = logicalDevice.addSingleQueue(graphicsQueueFamily, 1.0f);

		if (presentQueueFamily == graphicsQueueFamily) presentQueue = graphicsQueue;
		else presentQueue = logicalDevice.addSingleQueue(presentQueueFamily, 1.0f);

		if (computeQueueFamily == graphicsQueueFamily) computeQueue = graphicsQueue;
		else if (computeQueueFamily == presentQueueFamily) computeQueue = presentQueue;
		else computeQueue = logicalDevice.addSingleQueue(computeQueueFamily, 1.0f);

		logicalDevice.commit(physicalDevice);
		std::cout << "Created logical device\n";
	}

	void getSwapchain()
	{
		
	}

	// ASSIST FUNCTIONS

	static bool doesDeviceSupportQueueFamilyTypes(svk::PhysicalDevice* device, bool types[4])
	{
		bool tmpTypes[4] = {types[0], types[1], types[2], types[3]};
		auto queueFamilies = device->getQueueFamilies();
		for (auto& queueFamily : queueFamilies)
		{
			for (int i = 0; i < 4; i++)
			{
				if (tmpTypes[i] && queueFamily.types[i])
					tmpTypes[i] = false;
			}
		}
		return std::ranges::all_of(tmpTypes, [](bool i) { return !i; });
	}

	bool doesDeviceContainPresentQueue(svk::PhysicalDevice* device)
	{
		auto queueFamilies = device->getQueueFamilies();
		for (auto& queueFamily : queueFamilies)
		{
			if (queueFamily.supportsPresent(&window.getSurface()))
				return true;
		}
		return false;
	}
};

