//
// Created by SATAN on 16/04/2023.
//

#include "CommandPool.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"

void CommandPool::commit(LogicalDevice* lDevice, FamilyQueueType type)
{
    this->lDevice = lDevice;

    vk::CommandPoolCreateInfo poolInfo{};
    auto index = lDevice->getPhysicalDevice()->getQueueFamilyIndices().getIndex(type);

    if (!index.has_value())
        // If this happens, make sure the queue has been selected in PhysicalDevice.setQueuesToUse
        throw std::runtime_error("Failed to find queue family index for type " + std::to_string(type));
    poolInfo.queueFamilyIndex = index.value();
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    
    commandPool = std::make_shared<vk::raii::CommandPool>(*lDevice->getRaiiHandle(), poolInfo);
}
