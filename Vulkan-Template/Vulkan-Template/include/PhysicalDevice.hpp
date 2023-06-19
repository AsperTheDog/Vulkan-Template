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

		VkPhysicalDeviceProperties getProperties() const;
		VkPhysicalDeviceFeatures getFeatures() const;
		VkPhysicalDeviceMemoryProperties getMemoryProperties() const;
		std::vector<QueueFamily> getQueueFamilies() const;
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

