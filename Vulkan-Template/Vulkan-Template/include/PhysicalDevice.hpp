#pragma once

#include <vector>

#include "SimpleVk_core.hpp"

namespace svk {
	class QueueFamily;
	class Surface;

	class PhysicalDevice
	{
	public:
		PhysicalDevice() = delete;

		bool areExtensionsSupported(std::vector<const char*> exts);
		bool isExtensionSupported(const char* ext);
		bool isFormatSupported(const Surface& surface, VkSurfaceFormatKHR format);
		bool isPresentModeSupported(const Surface& surface, VkPresentModeKHR presentMode);

		VkPhysicalDeviceProperties getProperties() const;
		VkPhysicalDeviceFeatures getFeatures() const;
		VkPhysicalDeviceMemoryProperties getMemoryProperties() const;
		std::vector<QueueFamily> getQueueFamilies() const;
		SurfaceProperties getSurfaceProperties(const Surface& surface) const;
		uint32_t getID() const { return id; }
		VkPhysicalDevice getVKHandle() const { return vkHandle; }

	private:
		explicit PhysicalDevice(VkPhysicalDevice device);

		VkPhysicalDevice vkHandle = nullptr;
		uint32_t id = 0;

		inline static uint32_t s_idCounter = 0;

		friend class Instance;
		friend class Surface;
	};
}

