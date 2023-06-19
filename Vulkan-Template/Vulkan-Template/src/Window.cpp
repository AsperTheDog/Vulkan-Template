#include "Window.hpp"

#include <stdexcept>

#include "Instance.hpp"
#include "SDL2/SDL_vulkan.h"

namespace svk {
	Window::Window(uint16_t width, uint16_t height, const char* name)
		: Window(width, height, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, name, SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED)
	{}

	Window::Window(uint16_t width, uint16_t height, const char* name, uint32_t flags)
		: Window(width, height, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, name, flags)
	{}

	Window::Window(uint16_t width, uint16_t height, uint16_t top, uint16_t left, const char* name, uint32_t flags)
	{
		size.width = width;
		size.height = height;

		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
		SDLHandle = SDL_CreateWindow(name, top, left, width, height, flags | SDL_WINDOW_VULKAN);
	}

	bool Window::isWindowOpen() const
	{
		return !wasQuitCalled;
	}

	void Window::processEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				wasQuitCalled = true;
		}
	}

	std::vector<const char*> Window::getRequiredExtensions() const
	{
		uint32_t extensionCount = 0;
		SDL_Vulkan_GetInstanceExtensions(SDLHandle, &extensionCount, nullptr);

		std::vector<const char*> extensions(extensionCount);
		SDL_Vulkan_GetInstanceExtensions(SDLHandle, &extensionCount, extensions.data());

		return extensions;
	}

	void Window::createSurface(svk::Instance* instance)
	{
		VkSurfaceKHR vkSurface;
		if (SDL_Vulkan_CreateSurface(SDLHandle, instance->getVkHandle(), &vkSurface) == SDL_FALSE)
			throw std::runtime_error("failed to create SDLHandle surface!");

		surface.reset(vkSurface, instance);
	}
}
