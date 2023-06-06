#include "QueueFamilies.hpp"

#include <set>
#include <utility>

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"

QueueFamilies::QueueFamilies(PhysicalDevice* pDevice)
	: surface(pDevice->getSurface()), physicalDevice(pDevice) {}

void QueueFamilies::commitQueues(LogicalDevice* device)
{
	for (auto& q : queues)
	{
		device->setQueue(q.queue.vkQueue, q.queue.index, 0);
	}
}

bool QueueFamilies::addQueue(const Queue& queue)
{
	for (auto& q : queues)
	{
		if (q.queue.index != queue.index)
			continue;

		q.queue.type |= queue.type;
		return false;
	}
	queues.push_back({queue, {1.0f}});
	return true;
}

void QueueFamilies::updateQueue(uint32_t index, const std::vector<float>& priorities)
{
	for (auto& q : queues)
	{
		if (q.queue.index != index)
			continue;

		q.priorities = priorities;
		q.queue.count = std::min((uint32_t) priorities.size(), q.queue.count);
		return;
	}
	std::cerr << "Queue not found\n";
}

std::vector<QueueFamilies::Queue> QueueFamilies::findQueues(FamilyQueueType type)
{
	auto queueFamilies = physicalDevice->getVKRaiiHandle()->getQueueFamilyProperties();
	uint32_t i = 0;

#ifdef _DEBUG
	if (!surface && type == FamilyQueuePresent)
		throw std::runtime_error("Requested present family but Surface is null");
#endif // DEBUG

	std::vector<Queue> queues{};
	for (const auto& queueFamily : queueFamilies)
	{
		if (type == FamilyQueueGraphics && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			queues.emplace_back(FamilyQueueGraphics, i, queueFamily.queueCount);
		else if (type == FamilyQueueCompute && queueFamily.queueFlags & vk::QueueFlagBits::eCompute)
			queues.emplace_back(FamilyQueueCompute, i, queueFamily.queueCount);
		else if (type == FamilyQueuePresent && physicalDevice->getVKRaiiHandle()->getSurfaceSupportKHR(i, **surface->getVKRaiiHandle()))
			queues.emplace_back(FamilyQueuePresent, i, queueFamily.queueCount);
	}
	return queues;
}

std::vector<QueueFamilies::QueueHolder> QueueFamilies::getQueues(FamilyQueueType type)
{
	std::vector<QueueHolder> ret{};
	for (auto& q : queues)
	{
		if (q.queue.type & type)
			ret.emplace_back(q);
	}
	return ret;
}

bool QueueFamilies::contains(FamilyQueueType type) const
{
	for (auto& q : queues)
	{
		if (q.queue.type & type)
			return true;
	}
	return false;
}

std::vector<uint32_t> QueueFamilies::getUniqueIndices()
{
	auto uniqueIndices = std::set<uint32_t>{};
	for (auto& queue : queues)
	{
		if (!uniqueIndices.contains(queue.queue.index))
			uniqueIndices.insert(queue.queue.index);
	}
	return std::vector(uniqueIndices.begin(), uniqueIndices.end());
}
