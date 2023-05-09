//
// Created by SATAN on 09/04/2023.
//

#include "Window.hpp"

#include "SDL2/SDL_vulkan.h"
#include "../VK/Instance.hpp"
#include "../VK/Surface.hpp"

Window::Window(uint16_t width, uint16_t height, const char* name, Uint32 flags) : size{ width, height }
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	window = SDL_CreateWindow("SDL Vulkan Sample", 0, 0, width, height, flags | SDL_WINDOW_VULKAN);
}

std::vector<const char*> Window::getRequiredExtensions()
{
	uint32_t count;
	SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr);
	std::vector<const char*> extensions(count);
	SDL_Vulkan_GetInstanceExtensions(window, &count, extensions.data());
	return extensions;
}

bool Window::isWindowOpen()
{
	return !wasQuitCalled;
}

void Window::processEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			wasQuitCalled = true;
			break;
		default:
			break;
		}
	}
}

void Window::commitSurface(Surface* surface, Instance* instance)
{
	VkSurfaceKHR vkSurface;
	if (SDL_Vulkan_CreateSurface(window, instance->getVKHandle(), &vkSurface) == SDL_FALSE)
        throw std::runtime_error("failed to create window surface!");

	surface->commit(this, vkSurface, instance);
}
