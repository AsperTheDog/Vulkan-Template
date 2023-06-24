#pragma once
#include "SimpleVk_core.hpp"

namespace svk {
	class Image;
	class LogicalDevice;

	class ImageView
	{
	public:
		~ImageView();

		void setType(VkImageViewType type) { this->type = type; }
		void setFormat(VkFormat format) { this->format = format; }
		void setComponentMapping(VkComponentSwizzle swizzle);
		void setComponentMapping(VkComponentMapping mapping);
		void setSubresourceRange(VkImageSubresourceRange range) { this->subresourceRange = range; }

		VkImageView getVKHandle() const { return vkHandle; }

	private:
		void commit(LogicalDevice* lDevice, Image* img);

		VkImageView vkHandle = nullptr;

		VkImageViewType type{};
		VkFormat format{};
		VkComponentMapping componentMapping{};
		VkImageSubresourceRange subresourceRange{};
		LogicalDevice* logicalDevice{};

		friend class Image;
	};

	class Image
	{
	public:
		void commit();

		void setImageView(LogicalDevice* lDevice, const ImageView& view);

		VkImage getVKHandle() const { return vkHandle; }

	private:
		explicit Image(VkImage vkHandle);

		VkImage vkHandle = nullptr;
		svk::ImageView view;

		friend class Swapchain;
	};
}
