//
// Created by SATAN on 16/04/2023.
//

#include "CommandPool.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"

void CommandPool::commit(LogicalDevice* lDevice)
{
    this->lDevice = lDevice;

    vk::CommandPoolCreateInfo poolInfo{};
    poolInfo.queueFamilyIndex = lDevice->getPhysicalDevice()->getQueueFamilyIndices().graphicsFamily.value();
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    
    commandPool = std::make_shared<vk::raii::CommandPool>(*lDevice->getRaiiHandle(), poolInfo);
}
