//
// Created by SATAN on 16/04/2023.
//
#pragma once


#include <vulkan/vulkan_raii.hpp>

#include "PhysicalDevice.hpp"

class LogicalDevice;

class CommandPool
{
public:
    CommandPool() = default;

    void commit(LogicalDevice* lDevice, FamilyQueueType type, int queueNumber);
    
    std::shared_ptr<vk::raii::CommandPool> getVKRaiiHandle() const { return commandPool; }
    VkCommandPool getVKBaseHandle() const { return **commandPool; }

    LogicalDevice* getLogicalDevice() const { return logicalDevice; }
    
private:
    LogicalDevice* logicalDevice = nullptr;

    std::shared_ptr<vk::raii::CommandPool> commandPool;
};
