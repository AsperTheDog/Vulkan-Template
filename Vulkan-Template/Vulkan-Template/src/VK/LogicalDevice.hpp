//
// Created by SATAN on 12/04/2023.
//
#pragma once


#include <memory>
#include <vulkan/vulkan_raii.hpp>

class Fence;
class PhysicalDevice;

class LogicalDevice
{
public:
    LogicalDevice() = default;

    LogicalDevice(LogicalDevice&& other) noexcept;

    void commit(PhysicalDevice* pDevice);
    
    std::shared_ptr<vk::raii::Device> getRaiiHandle() const { return device; }
    VkDevice getVKHandle() const { return **device; }

    PhysicalDevice* getPhysicalDevice() const { return pDevice; }

    vk::raii::Queue getGraphicsQueue() const { return graphicsQueue; }
    vk::raii::Queue getPresentQueue() const { return presentQueue; }

    void submitToQueue(std::vector<vk::SubmitInfo> info, Fence* fence);
    void presentInQueue(vk::PresentInfoKHR* info);
    void waitIdle();

private:
    PhysicalDevice* pDevice;
    std::shared_ptr<vk::raii::Device> device;
    
    vk::raii::Queue graphicsQueue = nullptr;
    vk::raii::Queue presentQueue = nullptr;
};
