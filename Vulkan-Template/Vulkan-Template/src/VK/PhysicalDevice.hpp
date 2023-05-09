//
// Created by SATAN on 12/04/2023.
//
#pragma once

#include <vulkan/vulkan_raii.hpp>
#include <optional>

class Surface;
class Instance;

enum FamilyQueueType
{
	FamilyQueueGraphics,
    FamilyQueuePresent,
    FamilyQueueCompute
};

struct RequiredFamilyQueues
{
	bool graphics = false;
    bool present = false;
    bool compute = false;
};

struct FamilyQueueIndices
{
    FamilyQueueIndices() = default;
    FamilyQueueIndices(uint32_t graphicsFamily, uint32_t presentFamily, uint32_t computeFamily) :
        graphicsFamily(graphicsFamily), presentFamily(presentFamily), computeFamily(computeFamily) {}
    
    bool isComplete(RequiredFamilyQueues requiredTypes) const
    {
	    return (graphicsFamily.has_value() || !requiredTypes.graphics) && 
			   (presentFamily.has_value() || !requiredTypes.present) &&
               (computeFamily.has_value() || !requiredTypes.compute);
    }

    std::optional<uint32_t> getIndex(FamilyQueueType type)
    {
	    switch (type)
	    {
	    case FamilyQueueGraphics:
				return graphicsFamily;
            case FamilyQueuePresent:
				return presentFamily;
            case FamilyQueueCompute:
                return computeFamily;
	    }
	    return {};
    }

    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> computeFamily;
};

class PhysicalDevice
{
public:
    // bool example(const PhysicalDevice& device)
    typedef bool (*SuitableType)(const PhysicalDevice&);

    PhysicalDevice(vk::raii::PhysicalDevice device, Instance* instance);
    
    vk::raii::PhysicalDevice* getRaiiHandle() { return &physicalDevice; }
    VkPhysicalDevice getVKHandle() const { return *physicalDevice; }

    void setQueuesToUse(RequiredFamilyQueues requiredQueues);
    void setSurface(Surface* surface) { this->surface = surface; }

    void addSuitableCheck(SuitableType check) { this->check = check; }
    bool isSuitable();

    FamilyQueueIndices getQueueFamilyIndices();
    FamilyQueueIndices getQueueFamilyIndices(RequiredFamilyQueues requiredTypes);

    std::vector<vk::SurfaceFormatKHR> getSupportedFormats();
    std::vector<vk::PresentModeKHR> getSupportedPresentModes();
    vk::SurfaceCapabilitiesKHR getSurfaceCapabilities();

    vk::PhysicalDeviceProperties getProperties() const { return this->physicalDevice.getProperties(); }
    vk::PhysicalDeviceFeatures getFeatures() const { return this->physicalDevice.getFeatures(); }

private:
    RequiredFamilyQueues requiredQueues{};
    Surface* surface = nullptr;

    SuitableType check = nullptr;
    Instance* instance{};
    vk::raii::PhysicalDevice physicalDevice;
};
