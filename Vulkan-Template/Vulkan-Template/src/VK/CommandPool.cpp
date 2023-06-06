//
// Created by SATAN on 16/04/2023.
//

#include "CommandPool.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"

void CommandPool::commit(LogicalDevice* lDevice, FamilyQueueType type, int queueNumber)
{
    this->logicalDevice = lDevice;

    vk::CommandPoolCreateInfo poolInfo{};
    auto index = lDevice->getPhysicalDevice()->getQueueFamilies()->getQueues(type)[queueNumber];
    
    poolInfo.queueFamilyIndex = index.queue.index;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    
    commandPool = std::make_shared<vk::raii::CommandPool>(*lDevice->getVKRaiiHandle(), poolInfo);
}
