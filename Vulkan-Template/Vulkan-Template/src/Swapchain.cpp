#include "Swapchain.hpp"

#include <stdexcept>

#include "LogicalDevice.hpp"
#include "Surface.hpp"

namespace svk
{
	Swapchain::~Swapchain()
	{
		vkDestroySwapchainKHR(logicalDevice->getVKHandle(), vkHandle, nullptr);
	}

	void Swapchain::commit(LogicalDevice& lDevice, const Surface& surface)
	{
		this->logicalDevice = &lDevice;
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface.getVKHandle();
		createInfo.minImageCount = desiredImgCount;
		createInfo.imageFormat = desiredFormat.format;
		createInfo.imageColorSpace = desiredFormat.colorSpace;
		createInfo.imageExtent = desiredExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = (VkSharingMode)desiredMode;
		auto uniqueIndices = lDevice.getUniqueQueueFamilyIndices();
		if (desiredMode == svk::QueueModes::CONCURRENT)
		{
			createInfo.queueFamilyIndexCount = (uint32_t)uniqueIndices.size();
			createInfo.pQueueFamilyIndices = uniqueIndices.data();
		}
		createInfo.preTransform = this->desiredPreTransform;
		createInfo.compositeAlpha = this->desiredCompositeAlpha;
		createInfo.presentMode = this->desiredPresentMode;
		createInfo.oldSwapchain = oldSwapchain == nullptr ? VK_NULL_HANDLE : oldSwapchain;

		if (vkCreateSwapchainKHR(lDevice.getVKHandle(), &createInfo, nullptr, &vkHandle) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}

	}
}
