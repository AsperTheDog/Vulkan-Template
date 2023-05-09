//
// Created by SATAN on 12/04/2023.
//

#include "Swapchain.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"
#include "../SDL/Window.hpp"
#include "Synchronization/Semaphore.hpp"

void Swapchain::commit(LogicalDevice* lDevice, Surface* surface)
{
    this->lDevice = lDevice;

    // Query the formats and present modes of the swapchain
    auto formats = lDevice->getPhysicalDevice()->getSupportedFormats();
    auto presentModes = lDevice->getPhysicalDevice()->getSupportedPresentModes();
    
    // Look for the preffered formats in order of preference
    vk::SurfaceFormatKHR surfaceFormat{};
    for (auto & format : preferredFormats)
    {
        for (auto & availableFormat : formats)
        {
            if (format.format == availableFormat.format && format.colorSpace == availableFormat.colorSpace)
            {
                surfaceFormat = format;
                break;
            }
        }
        if (surfaceFormat.format != vk::Format::eUndefined)
            break;
    }
    if (surfaceFormat.format == vk::Format::eUndefined)
        surfaceFormat = formats[0];
    
    // Look for the preffered present modes in order of preference
    vk::PresentModeKHR presentMode{};
    for (auto & mode : preferredPresentModes)
    {
        for (auto & availableMode : presentModes)
        {
            if (mode == availableMode)
            {
                presentMode = mode;
                break;
            }
        }
        if (presentMode != vk::PresentModeKHR{})
            break;
    }
    if (presentMode == vk::PresentModeKHR{})
        presentMode = vk::PresentModeKHR::eFifo;
    
    this->format = surfaceFormat;
    this->presentMode = presentMode;
    
    // Choose the swap extent
    auto capabilities = lDevice->getPhysicalDevice()->getSurfaceCapabilities();
    vk::Extent2D swapchainExtent{};
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        swapchainExtent = capabilities.currentExtent;
    else
    {
        swapchainExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, surface->getWindow()->getSize().width));
        swapchainExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, surface->getWindow()->getSize().height));
    }
    
    this->extent = vk::Extent2D(swapchainExtent.width, swapchainExtent.height);
    
    vk::SwapchainCreateInfoKHR swapchainCreateInfo{};
    swapchainCreateInfo.surface = surface->getVKHandle();
    swapchainCreateInfo.minImageCount = capabilities.minImageCount + 1;

    // Make sure the image count is not greater than the maximum
    if (capabilities.maxImageCount > 0 && swapchainCreateInfo.minImageCount > capabilities.maxImageCount)
        swapchainCreateInfo.minImageCount = capabilities.maxImageCount;
    
    swapchainCreateInfo.imageFormat = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = swapchainExtent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
    
    // Check if the swapchain needs to be shared between queues
    FamilyQueueIndices indices = lDevice->getPhysicalDevice()->getQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    if (indices.graphicsFamily != indices.presentFamily)
    {
        swapchainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
        swapchainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
    
    swapchainCreateInfo.preTransform = capabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = nullptr;
    
    this->swapchain = std::make_shared<vk::raii::SwapchainKHR>(*lDevice->getRaiiHandle(), swapchainCreateInfo);
    this->swapchainImages = swapchain->getImages();

    // Create the image views
    vk::ImageViewCreateInfo imageViewCreateInfo{};
    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
    imageViewCreateInfo.format = surfaceFormat.format;
    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;
    for (auto image: swapchainImages)
    {
        imageViewCreateInfo.image = image;
        swapchainImageViews.emplace_back(*lDevice->getRaiiHandle(), imageViewCreateInfo);
    }
}

void Swapchain::generateFBs(RenderPass* renderPass)
{
    frameBuffers.reserve(swapchainImages.size());
    for (auto & imageView : swapchainImageViews)
    {
        frameBuffers.emplace_back();
        frameBuffers.back().commit(imageView, renderPass);
    }
}

FrameBuffer* Swapchain::getActiveFramebuffer()
{
    return &frameBuffers[imageIdx];
}

uint32_t Swapchain::acquireNextImage(Semaphore* sem)
{
    auto [result, value] = swapchain->acquireNextImage(
        std::numeric_limits<uint64_t>::max(), 
        sem->getVKHandle());
    if (result != vk::Result::eSuccess)
		throw std::runtime_error("Failed to acquire next image");
    return value;
}

void Swapchain::setActiveIndex(uint32_t index)
{
    imageIdx = index;
}
