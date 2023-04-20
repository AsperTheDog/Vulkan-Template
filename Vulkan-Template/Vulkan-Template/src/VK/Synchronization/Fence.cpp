#include "Fence.hpp"
#include "../LogicalDevice.hpp"

void Fence::commit(LogicalDevice* lDevice)
{
	this->lDevice = lDevice;
	vk::FenceCreateInfo fenceInfo{};
	fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

	fence = std::make_shared<vk::raii::Fence>(*lDevice->getRaiiHandle(), fenceInfo);
}

void Fence::wait()
{
	if (lDevice->getRaiiHandle()->waitForFences(**fence, VK_TRUE, UINT64_MAX) != vk::Result::eSuccess)
		throw std::runtime_error("Failed to wait for fence!");
}

void Fence::reset()
{
	lDevice->getRaiiHandle()->resetFences(**fence);
}
