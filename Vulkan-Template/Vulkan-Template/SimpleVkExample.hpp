#pragma once
#include <algorithm>
#include <iostream>

#include <vulkan/vk_enum_string_helper.h>

#include "SimpleVk.hpp"
#include "Window.hpp"

class SimpleVkExample
{
public:

	SimpleVkExample() : window(1920, 1080, "SimpleVk Example")
	{
		std::cout << "Initiated window\n\n";
		this->initInstance();
		this->createSurface();
		this->getPhysicalDevice();
		this->getLogicalDevice();
		this->getSwapchain();
	}

private:
	svk::Instance instance;
	svk::Window window;
	svk::PhysicalDevice* physicalDevice{};
	svk::LogicalDevice logicalDevice;
	svk::Swapchain swapchain;

	svk::QueuePosition graphicsQueue{};
	svk::QueuePosition presentQueue{};
	svk::QueuePosition computeQueue{};
	uint8_t uniqueQueueNum = 0;

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
		instance.commit(svk::VulkanApiVersion::V_1_3);

		std::cout << "Created instance (using version 1.3.0)\n\n";
	}

	void createSurface()
	{
		window.createSurface(&instance);
		std::cout << "Created surface\n\n";
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
				std::cerr << " - UNSUITABLE: Device is not a discrete GPU\n";
				continue;
			}
			std::cout << " - Device is a discrete GPU\n";

			if (properties.apiVersion < instance.getApiVersion())
			{
				std::string api = std::to_string(VK_VERSION_MAJOR(instance.getApiVersion()))
					+ "." + std::to_string(VK_VERSION_MINOR(instance.getApiVersion()))
					+ "." + std::to_string(VK_VERSION_PATCH(instance.getApiVersion()));
				std::cerr << " - UNSUITABLE: Device does not support Vulkan " << api << "\n";
				continue;
			}
			std::string api = std::to_string(VK_VERSION_MAJOR(properties.apiVersion))
				+ "." + std::to_string(VK_VERSION_MINOR(properties.apiVersion))
				+ "." + std::to_string(VK_VERSION_PATCH(properties.apiVersion));
			std::cout << " - Device supports Vulkan " << api << "\n";

			if (!device.areExtensionsSupported(requiredExts))
			{
				std::cerr << " - UNSUITABLE: Device does not support the necessary extensions\n";
				continue;
			}
			std::cout << " - Device supports all necessary extensions\n";

			svk::QueueTypeSupport types {true, true, false, false};
			if (!doesDeviceSupportQueueFamilyTypes(&device, types))
			{
				std::cerr << " - UNSUITABLE: Device does not support required queue family types\n";
				continue;
			}
			std::cout << " - Device supports required queue family types\n";

			if (!doesDeviceContainPresentQueue(&device))
			{
				std::cerr << " - UNSUITABLE: Device does not contain a queue family that supports presentation\n";
				continue;
			}
			std::cout << " - Device contains a queue family that supports presentation\n";

			if (!device.isFormatSupported(window.getSurface(), { VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }))
			{
				std::cerr << " - UNSUITABLE: Device does not support desired formats\n";
				continue;
			}
			std::cout << " - Device supports desired formats\n";

			if (!device.isPresentModeSupported(window.getSurface(), VK_PRESENT_MODE_MAILBOX_KHR))
			{
				std::cerr << " - UNSUITABLE: Device does not support desired present mode\n";
				continue;
			}
			std::cout << " - Device supports desired present mode\n";

			physicalDevice = &device;
			break;
		}

		if (physicalDevice == nullptr)
		{
			throw std::runtime_error("No suitable physical device found");
		}

		std::cout << "Device is suitable, selected for use\n\n";
	}

	void getLogicalDevice()
	{
#ifdef _DEBUG
		logicalDevice.addValidationLayers();
#endif

		for (auto& ext : requiredExts)
			logicalDevice.addExtension(ext);

		auto queues = physicalDevice->getQueueFamilies();
		std::optional<svk::QueueFamily> graphicsQueueFamily{};
		std::optional<svk::QueueFamily> presentQueueFamily{};
		std::optional<svk::QueueFamily> computeQueueFamily{};
		for (auto& queue : queues)
		{
			if (queue.types.graphics && !graphicsQueueFamily.has_value())
			{
				std::cout << "Found graphics queue family with index " << queue.index << "\n";
				graphicsQueueFamily = std::make_optional(queue);
			}
			if (queue.supportsPresent(&window.getSurface()) && !presentQueueFamily.has_value())
			{
				std::cout << "Found present queue family with index " << queue.index << "\n";
				presentQueueFamily = std::make_optional(queue);
			}
			if (queue.types.compute && !computeQueueFamily.has_value())
			{
				std::cout << "Found compute queue family with index " << queue.index << "\n";
				computeQueueFamily = std::make_optional(queue);
			}
		}
		
		if (!graphicsQueueFamily.has_value() || !presentQueueFamily.has_value() || !computeQueueFamily.has_value())
			throw std::runtime_error("Failed to find all required queue families");


		graphicsQueue = logicalDevice.addSingleQueue(*graphicsQueueFamily, 1.0f);
		uniqueQueueNum++;

		if (presentQueueFamily.value().index == graphicsQueueFamily.value().index) presentQueue = graphicsQueue;
		else
		{
			presentQueue = logicalDevice.addSingleQueue(*presentQueueFamily, 1.0f);
			uniqueQueueNum++;
		}

		if (computeQueueFamily.value().index == graphicsQueueFamily.value().index) computeQueue = graphicsQueue;
		else if (computeQueueFamily.value().index == presentQueueFamily.value().index) computeQueue = presentQueue;
		else
		{
			computeQueue = logicalDevice.addSingleQueue(*computeQueueFamily, 1.0f);
			uniqueQueueNum++;
		}

		logicalDevice.commit(physicalDevice);
		std::cout << "Created logical device\n\n";
	}

	void getSwapchain()
	{
		auto desiredExtent = findSwapchainExtent();
		auto desiredImageCount = findSwapchainImgCount();
		auto desiredPreTransform = window.getSurface().getProperties(physicalDevice).capabilities.currentTransform;

		swapchain.setDesiredFormat(VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
		swapchain.setDesiredPresentMode(VK_PRESENT_MODE_MAILBOX_KHR);
		swapchain.setExtent(desiredExtent);
		std::cout << "Swapchain extent set to " << desiredExtent.width << "x" << desiredExtent.height << "\n";
		swapchain.setImageCount(desiredImageCount);
		std::cout << "Swapchain image count set to " << desiredImageCount << "\n";
		if (uniqueQueueNum < 2)
		{
			swapchain.setQueueMode(svk::QueueModes::EXCLUSIVE);
			std::cout << "Swapchain queue mode set to EXCLUSIVE for 1 or less queues\n";
		}
		else
		{
			swapchain.setQueueMode(svk::QueueModes::CONCURRENT);
			std::cout << "Swapchain queue mode set to CONCURRENT for " << (uint32_t)uniqueQueueNum << " queues\n";
		}
		swapchain.setPreTransform(desiredPreTransform);
		std::cout << "Swapchain Pre transform set to " << string_VkSurfaceTransformFlagBitsKHR(desiredPreTransform) << "\n";
		swapchain.setCompositeAlpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR);
		swapchain.setClipped(true);

		swapchain.commit(logicalDevice, window.getSurface());
		std::cout << "Created swapchain\n\n";
	}

	// ASSIST FUNCTIONS

	static bool doesDeviceSupportQueueFamilyTypes(svk::PhysicalDevice* device, svk::QueueTypeSupport types)
	{
		auto queueFamilies = device->getQueueFamilies();
		for (auto& queueFamily : queueFamilies)
		{
			if (types.graphics && queueFamily.types.graphics) types.graphics = false;
			if (types.compute && queueFamily.types.compute) types.compute = false;
			if (types.transfer && queueFamily.types.transfer) types.transfer = false;
			if (types.sparseBinding && queueFamily.types.sparseBinding) types.sparseBinding = false;
		}
		return types.areAllFalse();
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

	VkExtent2D findSwapchainExtent()
	{
		auto capabilities = window.getSurface().getProperties(physicalDevice).capabilities;
		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;

		auto size = window.getSize();
		VkExtent2D extent = { size.height, size.width };

		extent.width = std::clamp(extent.width,
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width);

		extent.height = std::clamp(extent.height,
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height);

		return extent;
	}

	uint32_t findSwapchainImgCount()
	{
		auto capabilities = window.getSurface().getProperties(physicalDevice).capabilities;
		uint32_t imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
			imageCount = capabilities.maxImageCount;
		return imageCount;
	}
};

