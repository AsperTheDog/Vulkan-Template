#pragma once
#include <functional>
#include <vulkan/vulkan_core.h>

namespace svk
{
	enum VulkanApiVersion
	{
		V_1_0 = VK_API_VERSION_1_0,
		V_1_1 = VK_API_VERSION_1_1,
		V_1_2 = VK_API_VERSION_1_2,
		V_1_3 = VK_API_VERSION_1_3
	};

	enum QueueFamilyTypes
	{
		GRAPHICS,
		COMPUTE,
		TRANSFER,
		BINDING
	};

	struct QueuePosition
	{
		unsigned index;
		unsigned offset;

		bool operator==(const QueuePosition& other) const
		{
			return other.index == this->index && other.offset == this->offset;
		}
	};
}

template <>
struct std::hash<svk::QueuePosition>
{
	std::size_t operator()(const svk::QueuePosition& k) const noexcept
	{
		return (size_t)k.index << 32 | k.offset;
	}
};
