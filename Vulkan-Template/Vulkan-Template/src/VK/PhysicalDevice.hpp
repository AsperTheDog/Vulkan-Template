//
// Created by SATAN on 12/04/2023.
//
#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "QueueFamilies.hpp"

class Surface;
class Instance;

class PhysicalDevice
{
public:
    // bool example(const PhysicalDevice& device)
    typedef bool (*SuitableType)(const PhysicalDevice&);

    PhysicalDevice(vk::raii::PhysicalDevice device, Instance* instance);

    void setQueuesToUse(QueueFamilies requiredQueues) { queues = requiredQueues; }
    void setSurface(Surface* surface) { this->surface = surface; }

    // GETTERS
    
    vk::raii::PhysicalDevice* getVKRaiiHandle() { return &vkraiiHandle; }
    VkPhysicalDevice getVKBaseHandle() const { return *vkraiiHandle; }

    QueueFamilies* getQueueFamilies() { return &queues; }
    std::vector<vk::SurfaceFormatKHR> getSupportedFormats();
    std::vector<vk::PresentModeKHR> getSupportedPresentModes();
    vk::SurfaceCapabilitiesKHR getSurfaceCapabilities();
    uint32_t getApiVersion();
    Surface* getSurface() {return surface; }

    vk::PhysicalDeviceProperties getProperties() const { return this->vkraiiHandle.getProperties(); }
    vk::PhysicalDeviceFeatures getFeatures() const { return this->vkraiiHandle.getFeatures(); }

private:
    QueueFamilies queues;
    Surface* surface = nullptr;

    SuitableType check = nullptr;
    Instance* instance{};
    vk::raii::PhysicalDevice vkraiiHandle;

    friend class LogicalDevice;
};
