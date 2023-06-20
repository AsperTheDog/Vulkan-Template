#pragma once

#include "SimpleVk_core.hpp"

namespace svk {
	class Surface;
	class PhysicalDevice;

	class Swapchain
	{
	public:
		void commit(PhysicalDevice* pDevice, Surface& surface);

		void setDesiredFormat(VkFormat format, VkColorSpaceKHR colorSpace) { this->desiredFormat = {format, colorSpace}; }
		void setDesiredPresentMode(VkPresentModeKHR presentMode) { this->desiredPresentMode = presentMode; }
		void setExtent(VkExtent2D extent) { this->extent = extent; }
		void setImageCount(uint32_t imgCount) { this->desiredImgCount = imgCount; }

	private:
		VkSurfaceFormatKHR desiredFormat{};
		VkPresentModeKHR desiredPresentMode{};
		VkExtent2D extent{};
		uint32_t desiredImgCount{};
	};
}

