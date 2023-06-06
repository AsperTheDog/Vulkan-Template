#pragma once
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

class PhysicalDevice;
class LogicalDevice;
class Surface;

typedef uint8_t FamilyQueueTypeBits;
enum FamilyQueueType
{
	FamilyQueueGraphics = 1,
    FamilyQueuePresent = 2,
    FamilyQueueCompute = 4
};

class QueueFamilies
{
public:
	struct Queue
	{
        FamilyQueueTypeBits type;
		uint32_t index{};
		uint32_t count = 1;
        vk::raii::Queue vkQueue = nullptr;
	};

    struct QueueHolder
    {
	    Queue queue;
        std::vector<float> priorities;
    };

	explicit QueueFamilies(PhysicalDevice* pDevice);

    void commitQueues(LogicalDevice* device);

    bool addQueue(const Queue& queue);
    void updateQueue(uint32_t index, const std::vector<float>& priorities);

    std::vector<Queue> findQueues(FamilyQueueType type);

    std::vector<QueueHolder> getQueues(FamilyQueueType type);
    std::vector<QueueHolder>* getQueues() { return &queues; }

	bool contains(FamilyQueueType type) const;
	std::vector<uint32_t> getUniqueIndices();

	Surface* surface{};
    PhysicalDevice* physicalDevice{};

private:
    QueueFamilies() = default;

    std::vector<QueueHolder> queues;

    friend class PhysicalDevice;
    friend class LogicalDevice;
};
