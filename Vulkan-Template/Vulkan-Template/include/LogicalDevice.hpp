#pragma once

#include <unordered_map>
#include <vector>

#include "Queue.hpp"
#include "SimpleVk_core.hpp"

namespace svk
{
	class PhysicalDevice;

	class LogicalDevice
	{
	public:
		~LogicalDevice();

		void commit(PhysicalDevice* pDevice);

		QueuePosition addQueues(QueueFamily pQueueFamily, uint32_t queueCount, std::vector<float> pQueuePriorities);
		QueuePosition addSingleQueue(QueueFamily pQueueFamily, float priority);
		void setEnabledFeatures(VkPhysicalDeviceFeatures features){ enabledFeatures = features; }

		void addExtension(const char* extension);
		void addLayer(const char* layer);
		void addValidationLayers();

		std::vector<uint32_t> getUniqueQueueFamilyIndices();
		VkDevice getVKHandle() const { return vkHandle; }

	private:
		struct QueueCreateInfo
		{
			QueueFamily queueFamily;
			uint32_t queueCount;
			std::vector<float> pQueuePriorities;
		};

		VkDevice vkHandle = nullptr;

		VkPhysicalDeviceFeatures enabledFeatures{};
		std::vector<const char*> enabledExtensions;
		std::vector<const char*> enabledLayers;

		std::vector<QueueCreateInfo> queues;
		std::unordered_map<QueuePosition, Queue> queueHandles;

		uint32_t getQueuePosFromIndex(uint32_t familyIndex);
		void generateQueue(QueuePosition index);
	};
}
