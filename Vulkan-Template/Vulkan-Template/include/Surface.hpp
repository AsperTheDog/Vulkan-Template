#pragma once

#include "Instance.hpp"
#include "SimpleVk_core.hpp"

namespace svk {
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

