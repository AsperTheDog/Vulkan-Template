#include "LogicalDevice.hpp"

#include <stdexcept>

#include "PhysicalDevice.hpp"
#include "Queue.hpp"

namespace svk
{
	LogicalDevice::~LogicalDevice()
	{
		vkDestroyDevice(vkHandle, nullptr);
	}

	void LogicalDevice::commit(PhysicalDevice* pDevice)
	{

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		for (auto& queue : queues)
		{
			queueCreateInfos.push_back({});
			auto& queueCreateInfo = queueCreateInfos.back();
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queue.pQueueFamily->index;
			queueCreateInfo.queueCount = queue.queueCount;
			queueCreateInfo.pQueuePriorities = queue.pQueuePriorities.data();
		}
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		if (!queueCreateInfos.empty())
			createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
		if (!enabledExtensions.empty())
			createInfo.ppEnabledExtensionNames = enabledExtensions.data();

        createInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
		if (!enabledLayers.empty())
			createInfo.ppEnabledLayerNames = enabledLayers.data();
		
        createInfo.pEnabledFeatures = &enabledFeatures;

		if (vkCreateDevice(pDevice->getVKHandle(), &createInfo, nullptr, &vkHandle) != VK_SUCCESS) 
            throw std::runtime_error("failed to create logical device!");

		for (auto& queue : queues)
		{
			for (uint32_t i = 0; i < queue.queueCount; i++)
			{
				generateQueue({queue.pQueueFamily->index, i});
			}
		}
	}

	QueuePosition LogicalDevice::addQueues(QueueFamily* pQueueFamily, uint32_t queueCount, std::vector<float> pQueuePriorities)
	{
		uint32_t existingQueuePos = getQueuePosFromIndex(pQueueFamily->index);
		if (existingQueuePos >= queues.size())
		{
			queues.emplace_back(pQueueFamily, queueCount, pQueuePriorities);
			return {(uint32_t)(queues.size() - 1), 0};
		}
		auto qPos = queues[existingQueuePos].queueCount;
		queues[existingQueuePos].queueCount += queueCount;
		for (auto priority : pQueuePriorities)
			queues[existingQueuePos].pQueuePriorities.push_back(priority);
		return {existingQueuePos, qPos};
	}

	QueuePosition LogicalDevice::addSingleQueue(QueueFamily* pQueueFamily, float priority)
	{
		uint32_t existingQueuePos = getQueuePosFromIndex(pQueueFamily->index);
		if (existingQueuePos >= queues.size())
		{
			std::vector<float> priorities = {priority};
			queues.emplace_back(pQueueFamily, 1, priorities);
			return {(uint32_t)(queues.size() - 1), 0};
		}
		auto qPos = queues[existingQueuePos].queueCount++;
		queues[existingQueuePos].pQueuePriorities.push_back(priority);
		return {existingQueuePos, qPos};
	}

	uint32_t LogicalDevice::getQueuePosFromIndex(uint32_t familyIndex)
	{
		for (uint32_t i = 0; i < queues.size(); i++)
		{
			if (queues[i].pQueueFamily->index == familyIndex)
				return i;
		}
		return UINT32_MAX;
	}

	void LogicalDevice::generateQueue(QueuePosition index)
	{
		VkQueue queue;
		vkGetDeviceQueue(this->vkHandle, index.index, index.offset, &queue);
		Queue queueHandle{queue};
		queueHandles.emplace(index, queueHandle);
	}

	void LogicalDevice::addExtension(const char* extension)
	{
		enabledExtensions.push_back(extension);
	}

	void LogicalDevice::addLayer(const char* layer)
	{
		enabledLayers.push_back(layer);
	}

	void LogicalDevice::addValidationLayers()
	{
		addLayer("VK_LAYER_KHRONOS_validation");
	}
}
