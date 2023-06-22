#include "Queue.hpp"

#include "PhysicalDevice.hpp"
#include "Surface.hpp"

namespace svk
{
	bool QueueFamily::supportsPresent(const Surface* surface)
	{
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device->getVKHandle(), index, surface->getVKHandle(), &presentSupport);
		return presentSupport == VK_TRUE;
	}

	QueueFamily::QueueFamily(const PhysicalDevice* device, VkQueueFamilyProperties properties, uint32_t index)
	: types{ false, false, false, false }, properties(properties), index(index), device(device)
	{
		if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			types.graphics = true;
		else if (properties.queueFlags & VK_QUEUE_COMPUTE_BIT)
			types.compute = true;
		else if (properties.queueFlags & VK_QUEUE_TRANSFER_BIT)
			types.transfer = true;
		else if (properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
			types.sparseBinding = true;
	}
}
