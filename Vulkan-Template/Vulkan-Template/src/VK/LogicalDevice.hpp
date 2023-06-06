//
// Created by SATAN on 12/04/2023.
//
#pragma once

#include <memory>
#include <vulkan/vulkan_raii.hpp>

#include "QueueFamilies.hpp"

class Fence;
class PhysicalDevice;

struct LogicalDeviceQueue
{
	vk::raii::Queue queue = nullptr;
    FamilyQueueTypeBits type;
};

class LogicalDevice
{
public:
    void commit();
    
    std::shared_ptr<vk::raii::Device> getVKRaiiHandle() const { return device; }
    VkDevice getVKBaseHandle() const { return **device; }

    PhysicalDevice* getPhysicalDevice() const { return physicalDevice; }

    void setDeviceFeatures(vk::PhysicalDeviceFeatures features) { this->features = features; }
    void setQueuePriorities(uint32_t queueIndex, const std::vector<float>& priorities);

    void submitToQueue(FamilyQueueType type, uint32_t offset, const std::vector<vk::SubmitInfo>& info, Fence* fence);
    void presentInQueue(uint32_t offset, vk::PresentInfoKHR* info);
    void waitIdle();

    void setQueue(vk::raii::Queue& queue, uint32_t index, int i);
    void setPhysicalDevice(PhysicalDevice* pDevice) { physicalDevice = pDevice; }

private:
    PhysicalDevice* physicalDevice = nullptr;
    std::shared_ptr<vk::raii::Device> device;

    vk::PhysicalDeviceFeatures features{};
};
