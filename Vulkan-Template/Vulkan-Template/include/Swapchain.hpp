#pragma once

namespace svk {
	class Surface;
	class PhysicalDevice;

	class Swapchain
	{
	public:
		void commit(PhysicalDevice* pDevice, Surface& surface);

	private:

	};
}

