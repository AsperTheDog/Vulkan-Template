//
// Created by SATAN on 12/04/2023.
//
#pragma once


#include <utility>
#include <vector>
#include "FrameBuffer.hpp"

class Surface;
class LogicalDevice;
class Semaphore;

class Swapchain
{
public:
    struct PresentProperties
    {
        vk::Format format;
        vk::PresentModeKHR presentMode;
    };

    explicit Swapchain() = default;

    void commit(LogicalDevice* lDevice, Surface* surface);
    void generateFBs(RenderPass* renderPass);

    void setPreferredFormat(std::vector<vk::SurfaceFormatKHR> formats) { preferredFormats = std::move(formats); }
    void setPreferredPresentMode(std::vector<vk::PresentModeKHR> modes) { preferredPresentModes = std::move(modes); }

    std::shared_ptr<vk::raii::SwapchainKHR> getRaiiHandle() const { return swapchain; }
    VkSwapchainKHR getVKHandle() const { return **swapchain; }

    LogicalDevice* getLogicalDevice() const { return lDevice; }

    vk::Extent2D getExtent() const { return extent; }
    PresentProperties getPresentProperties() const { return {format.format, presentMode}; }
    FrameBuffer* getActiveFramebuffer();

    uint32_t acquireNextImage(Semaphore* sem);
    void setActiveIndex(uint32_t index);

private:
    LogicalDevice* lDevice;
    std::shared_ptr<vk::raii::SwapchainKHR> swapchain;
    
    vk::Extent2D extent;
    
    std::vector<vk::Image> swapchainImages;
    std::vector<vk::raii::ImageView> swapchainImageViews;
    
    std::vector<FrameBuffer> frameBuffers;
    
    std::vector<vk::SurfaceFormatKHR> preferredFormats;
    std::vector<vk::PresentModeKHR> preferredPresentModes;
    
    uint32_t imageIdx;
    
    vk::SurfaceFormatKHR format;
    vk::PresentModeKHR presentMode;
};
