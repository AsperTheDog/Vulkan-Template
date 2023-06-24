#include "LogicalDevice.hpp"

#include <set>
#include <stdexcept>

#include "PhysicalDevice.hpp"
#include "Queue.hpp"

namespace svk
{
	LogicalDevice::~LogicalDevice()
	{
		vkDestroyDevice(this->vkHandle, nullptr);
	}

	void LogicalDevice::commit(PhysicalDevice* pDevice)
	{

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		for (auto& queue : this->queues)
		{
			queueCreateInfos.push_back({});
			auto& queueCreateInfo = queueCreateInfos.back();
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queue.queueFamily.index;
			queueCreateInfo.queueCount = queue.queueCount;
			queueCreateInfo.pQueuePriorities = queue.pQueuePriorities.data();
		}
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		if (!queueCreateInfos.empty())
			createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.enabledExtensionCount = static_cast<uint32_t>(this->enabledExtensions.size());
		if (!enabledExtensions.empty())
			createInfo.ppEnabledExtensionNames = this->enabledExtensions.data();

		createInfo.enabledLayerCount = static_cast<uint32_t>(this->enabledLayers.size());
		if (!enabledLayers.empty())
			createInfo.ppEnabledLayerNames = this->enabledLayers.data();

		createInfo.pEnabledFeatures = &this->enabledFeatures;

		if (vkCreateDevice(pDevice->getVKHandle(), &createInfo, nullptr, &this->vkHandle) != VK_SUCCESS)
			throw std::runtime_error("failed to create logical device!");

		for (auto& queue : this->queues)
		{
			for (uint32_t i = 0; i < queue.queueCount; i++)
			{
				generateQueue({ queue.queueFamily.index, i });
			}
		}
	}

	QueuePosition LogicalDevice::addQueues(QueueFamily pQueueFamily, uint32_t queueCount, std::vector<float> pQueuePriorities)
	{
		uint32_t existingQueuePos = getQueuePosFromIndex(pQueueFamily.index);
		if (existingQueuePos >= this->queues.size())
		{
			this->queues.emplace_back(pQueueFamily, queueCount, pQueuePriorities);
			return { (uint32_t)(this->queues.size() - 1), 0 };
		}
		auto qPos = this->queues[existingQueuePos].queueCount;
		this->queues[existingQueuePos].queueCount += queueCount;
		for (auto priority : pQueuePriorities)
			this->queues[existingQueuePos].pQueuePriorities.push_back(priority);
		return { existingQueuePos, qPos };
	}

	QueuePosition LogicalDevice::addSingleQueue(QueueFamily pQueueFamily, float priority)
	{
		uint32_t existingQueuePos = getQueuePosFromIndex(pQueueFamily.index);
		if (existingQueuePos >= this->queues.size())
		{
			std::vector<float> priorities = { priority };
			this->queues.emplace_back(pQueueFamily, 1, priorities);
			return { (uint32_t)(this->queues.size() - 1), 0 };
		}
		auto qPos = this->queues[existingQueuePos].queueCount++;
		this->queues[existingQueuePos].pQueuePriorities.push_back(priority);
		return { existingQueuePos, qPos };
	}

	uint32_t LogicalDevice::getQueuePosFromIndex(uint32_t familyIndex)
	{
		for (uint32_t i = 0; i < this->queues.size(); i++)
		{
			if (this->queues[i].queueFamily.index == familyIndex)
				return i;
		}
		return UINT32_MAX;
	}

	void LogicalDevice::generateQueue(QueuePosition index)
	{
		VkQueue queue;
		vkGetDeviceQueue(this->vkHandle, index.index, index.offset, &queue);
		Queue queueHandle{ queue };
		this->queueHandles.emplace(index, queueHandle);
	}

	void LogicalDevice::addExtension(const char* extension)
	{
		this->enabledExtensions.push_back(extension);
	}

	void LogicalDevice::addLayer(const char* layer)
	{
		this->enabledLayers.push_back(layer);
	}

	void LogicalDevice::addValidationLayers()
	{
		addLayer("VK_LAYER_KHRONOS_validation");
	}

	std::vector<uint32_t> LogicalDevice::getUniqueQueueFamilyIndices()
	{
		std::set<uint32_t> uniqueIndices{};
		for (auto& queue : this->queues)
		{
			uniqueIndices.insert(queue.queueFamily.index);
		}
		return std::vector<uint32_t>{uniqueIndices.begin(), uniqueIndices.end()};
	}
}
