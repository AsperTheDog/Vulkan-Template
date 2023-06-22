#pragma once

#include "SimpleVk_core.hpp"

namespace svk {
	class LogicalDevice;
	class Surface;

	class Swapchain
	{
	public:
		~Swapchain();

		void commit(LogicalDevice& lDevice, const Surface& surface);

		VkSwapchainKHR getVKHandle() const { return vkHandle; }

		void setDesiredFormat(VkFormat format, VkColorSpaceKHR colorSpace) { this->desiredFormat = {format, colorSpace}; }
		void setDesiredPresentMode(VkPresentModeKHR presentMode) { this->desiredPresentMode = presentMode; }
		void setExtent(VkExtent2D extent) { this->desiredExtent = extent; }
		void setImageCount(uint32_t imgCount) { this->desiredImgCount = imgCount; }
		void setQueueMode(svk::QueueModes mode) { this->desiredMode = mode; }
		void setPreTransform(VkSurfaceTransformFlagBitsKHR transform) { this->desiredPreTransform = transform; }
		void setCompositeAlpha(VkCompositeAlphaFlagBitsKHR compositeAlpha) { this->desiredCompositeAlpha = compositeAlpha; }
		void setClipped(bool clipped) { this->desiredClipped = clipped; }
		void setOldSwapchain(Swapchain* swapchain) { this->oldSwapchain = swapchain->getVKHandle(); }

	private:
		VkSwapchainKHR vkHandle = nullptr;

		VkSurfaceFormatKHR desiredFormat{};
		VkPresentModeKHR desiredPresentMode{};
		VkExtent2D desiredExtent{};
		uint32_t desiredImgCount{};
		svk::QueueModes desiredMode{};
		VkSurfaceTransformFlagBitsKHR desiredPreTransform{};
		VkCompositeAlphaFlagBitsKHR desiredCompositeAlpha{};
		bool desiredClipped{};
		VkSwapchainKHR oldSwapchain = nullptr;

		LogicalDevice* logicalDevice = nullptr;
	};
}

