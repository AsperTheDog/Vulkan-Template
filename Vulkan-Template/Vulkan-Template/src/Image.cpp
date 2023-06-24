#include "Image.hpp"

#include <stdexcept>

#include "LogicalDevice.hpp"

namespace svk
{
	void ImageView::commit(LogicalDevice* lDevice, Image* img)
	{
		this->logicalDevice = lDevice;

		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = img->getVKHandle();
		createInfo.viewType = type;
		createInfo.format = format;
		createInfo.components = componentMapping;
		createInfo.subresourceRange = subresourceRange;

		if (vkCreateImageView(lDevice->getVKHandle(), &createInfo, nullptr, &vkHandle) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}

	void Image::commit()
	{
	}

	void Image::setImageView(LogicalDevice* lDevice, const ImageView& view)
	{
		this->view = view;
		this->view.commit(lDevice, this);
	}

	Image::Image(VkImage vkHandle)
	{
		this->vkHandle = vkHandle;
	}

	ImageView::~ImageView()
	{
		if (this->logicalDevice == nullptr) return;
		vkDestroyImageView(this->logicalDevice->getVKHandle(), this->vkHandle, nullptr);
	}

	void ImageView::setComponentMapping(VkComponentSwizzle swizzle)
	{
		this->componentMapping.r = swizzle;
		this->componentMapping.g = swizzle;
		this->componentMapping.b = swizzle;
		this->componentMapping.a = swizzle;
	}

	void ImageView::setComponentMapping(VkComponentMapping mapping)
	{
		this->componentMapping = mapping;
	}
}
