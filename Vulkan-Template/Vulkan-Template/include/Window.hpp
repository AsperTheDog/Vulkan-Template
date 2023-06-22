#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "Surface.hpp"
#include "vulkan/vulkan_core.h"

namespace svk
{
	class Instance;
	class Window
	{
	public:
		struct WindowSize
		{
			uint32_t width;
			uint32_t height;
		};

		// INITIALIZERS

		Window(uint32_t width, uint32_t height, const char* name);
		Window(uint32_t width, uint32_t height, const char* name, uint32_t flags);
		Window(uint32_t width, uint32_t height, uint32_t top, uint32_t left, const char* name, uint32_t flags);
		~Window()
		{
			SDL_DestroyWindow(SDLHandle);
		}

		// EXECUTION FUNCTIONS

		bool isWindowOpen() const;
		void processEvents();

		// GETTERS

		WindowSize getSize() const { return size; }
		std::vector<const char*> getRequiredExtensions() const;
		const SDL_Window* getSDLHandle() const { return SDLHandle; }

		void createSurface(svk::Instance* instance);
		const Surface& getSurface() const { return surface; }

	private:
		svk::Surface surface{ nullptr, nullptr };

		WindowSize size{};
		SDL_Window* SDLHandle;

		bool wasQuitCalled = false;
	};
}
