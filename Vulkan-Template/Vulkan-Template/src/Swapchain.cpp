#include "Swapchain.hpp"

#include <stdexcept>

#include "LogicalDevice.hpp"
#include "Surface.hpp"
#include "Image.hpp"

namespace svk
{
	Swapchain::~Swapchain()
	{
		if (this->logicalDevice == nullptr) return;
		vkDestroySwapchainKHR(this->logicalDevice->getVKHandle(), this->vkHandle, nullptr);
	}

	void Swapchain::commit(LogicalDevice& lDevice, const Surface& surface)
	{
		this->logicalDevice = &lDevice;
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface.getVKHandle();
		createInfo.minImageCount = this->desiredImgCount;
		createInfo.imageFormat = this->desiredFormat.format;
		createInfo.imageColorSpace = this->desiredFormat.colorSpace;
		createInfo.imageExtent = this->desiredExtent;
		createInfo.imageArrayLayers = this->desiredLayers;
		createInfo.imageUsage = this->desiredUsage;
		createInfo.imageSharingMode = (VkSharingMode)this->desiredMode;
		auto uniqueIndices = lDevice.getUniqueQueueFamilyIndices();
		if (this->desiredMode == svk::QueueModes::CONCURRENT)
		{
			createInfo.queueFamilyIndexCount = (uint32_t)uniqueIndices.size();
			createInfo.pQueueFamilyIndices = uniqueIndices.data();
		}
		createInfo.preTransform = this->desiredPreTransform;
		createInfo.compositeAlpha = this->desiredCompositeAlpha;
		createInfo.presentMode = this->desiredPresentMode;
		createInfo.oldSwapchain = this->oldSwapchain == nullptr ? VK_NULL_HANDLE : this->oldSwapchain;

		if (vkCreateSwapchainKHR(lDevice.getVKHandle(), &createInfo, nullptr, &this->vkHandle) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}
		uint32_t imageCount = 0;
		vkGetSwapchainImagesKHR(lDevice.getVKHandle(), this->vkHandle, &imageCount, nullptr);
		std::vector<VkImage> images(imageCount);
		vkGetSwapchainImagesKHR(lDevice.getVKHandle(), this->vkHandle, &imageCount, images.data());
		for (auto image : images)
		{
			swapChainImages.push_back(Image(image));
		}
	}

	void Swapchain::generateImageViews(const ImageView& view)
	{
		for (auto& image : this->swapChainImages)
		{
			image.setImageView(this->logicalDevice, view);
		}
	}
}
