#pragma once

#include "SimpleVk_core.hpp"

namespace svk
{
	class PhysicalDevice;
	class Surface;

	class QueueFamily
	{
	public:
		bool supportsPresent(const Surface* surface);

		QueueTypeSupport types;
		VkQueueFamilyProperties properties;
		uint32_t index;

	private:
		const PhysicalDevice* device;

		explicit QueueFamily(const PhysicalDevice* device, VkQueueFamilyProperties properties, uint32_t index);

		friend class PhysicalDevice;
	};

	class Queue
	{
	public:
		Queue() = delete;
		
	private:
		explicit Queue(VkQueue queue) : queue(queue) {}
		VkQueue queue = nullptr;

		friend class LogicalDevice;
	};
}
