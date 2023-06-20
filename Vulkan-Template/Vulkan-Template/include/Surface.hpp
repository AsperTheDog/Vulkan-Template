#pragma once

#include <optional>

#include "Instance.hpp"
#include "SimpleVk_core.hpp"

namespace svk {
	class Surface
	{
	public:
		explicit Surface(VkSurfaceKHR surface, Instance* instance);
		~Surface();

		VkSurfaceKHR getVKHandle() const { return vkHandle; }
		SurfaceProperties getProperties(const PhysicalDevice* device) const;

	private:
		void reset(VkSurfaceKHR vk_surface, Instance* instance);

		VkSurfaceKHR vkHandle = nullptr;
		Instance* instance;

		friend class Window;
	};
}

