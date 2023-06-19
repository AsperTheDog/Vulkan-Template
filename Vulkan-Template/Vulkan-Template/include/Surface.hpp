#pragma once

#include "Instance.hpp"
#include "SimpleVk_core.hpp"

namespace svk {
	struct SurfaceProperties
	{
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats{};
		std::vector<VkPresentModeKHR> presentModes{};
	};

	class Surface
	{
	public:
		explicit Surface(VkSurfaceKHR surface, Instance* instance);
		~Surface();

		bool isDeviceCompatible(PhysicalDevice* device) const;
		VkSurfaceKHR getVKHandle() const { return vkHandle; }

	private:
		void reset(VkSurfaceKHR vk_surface, Instance* instance);

		VkSurfaceKHR vkHandle = nullptr;
		Instance* instance;

		friend class Window;
	};
}

