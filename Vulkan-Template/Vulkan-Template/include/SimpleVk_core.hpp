#pragma once
#include <functional>
#include <vector>
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

	enum QueueModes
	{
		CONCURRENT = VK_SHARING_MODE_CONCURRENT,
		EXCLUSIVE = VK_SHARING_MODE_EXCLUSIVE
	};

	struct QueueTypeSupport
	{
		bool graphics = false;
		bool compute = false;
		bool transfer = false;
		bool sparseBinding = false;

		bool areAllFalse()
		{
			return !compute && !graphics && !transfer && !sparseBinding;
		}
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

	struct SurfaceProperties
	{
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats{};
		std::vector<VkPresentModeKHR> presentModes{};
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
