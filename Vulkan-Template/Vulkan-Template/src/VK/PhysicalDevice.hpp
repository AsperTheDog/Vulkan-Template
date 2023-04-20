//
// Created by SATAN on 12/04/2023.
//
#pragma once


#include <memory>
#include <vulkan/vulkan_raii.hpp>
#include <optional>

#include "LogicalDevice.hpp"

class Surface;
class Instance;

struct FamilyQueueIndices
{
    FamilyQueueIndices() = default;
    FamilyQueueIndices(uint32_t graphicsFamily, uint32_t presentFamily) :
        graphicsFamily(graphicsFamily), presentFamily(presentFamily) {}
    
    bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
};

class PhysicalDevice
{
public:
    PhysicalDevice(vk::raii::PhysicalDevice device, Instance* instance);
    
    vk::raii::PhysicalDevice* getRaiiHandle() { return &physicalDevice; }
    VkPhysicalDevice getVKHandle() const { return *physicalDevice; }

    bool isSuitable();

    FamilyQueueIndices getQueueFamilyIndices();
    std::vector<vk::SurfaceFormatKHR> getSupportedFormats();
    std::vector<vk::PresentModeKHR> getSupportedPresentModes();
    vk::SurfaceCapabilitiesKHR getSurfaceCapabilities();

private:
    Instance* instance{};
    vk::raii::PhysicalDevice physicalDevice;
};
