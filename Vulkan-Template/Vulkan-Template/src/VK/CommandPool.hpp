//
// Created by SATAN on 16/04/2023.
//
#pragma once


#include <vulkan/vulkan_raii.hpp>

class LogicalDevice;

class CommandPool
{
public:
    CommandPool() = default;

    void commit(LogicalDevice* lDevice);
    
    std::shared_ptr<vk::raii::CommandPool> getRaiiHandle() const { return commandPool; }
    VkCommandPool getVKHandle() const { return **commandPool; }

    LogicalDevice* getLogicalDevice() const { return lDevice; }
    
private:
    LogicalDevice* lDevice = nullptr;

    std::shared_ptr<vk::raii::CommandPool> commandPool;
};
