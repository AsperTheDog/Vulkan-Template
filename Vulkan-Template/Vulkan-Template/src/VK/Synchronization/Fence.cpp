#include "Fence.hpp"
#include "../LogicalDevice.hpp"

void Fence::commit(LogicalDevice* lDevice)
{
	this->logicalDevice = lDevice;
	vk::FenceCreateInfo fenceInfo{};
	fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

	fence = std::make_shared<vk::raii::Fence>(*lDevice->getVKRaiiHandle(), fenceInfo);
}

void Fence::wait()
{
	if (logicalDevice->getVKRaiiHandle()->waitForFences(**fence, VK_TRUE, UINT64_MAX) != vk::Result::eSuccess)
		throw std::runtime_error("Failed to wait for fence!");
}

void Fence::reset()
{
	logicalDevice->getVKRaiiHandle()->resetFences(**fence);
}
