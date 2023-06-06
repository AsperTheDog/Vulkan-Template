//
// Created by SATAN on 09/04/2023.
//
#pragma once

#include <vector>
#include <SDL2/SDL.h>

class Instance;
class Surface;

class Window
{
public:
    struct WindowSize
    {
        uint32_t width;
        uint32_t height;
    };

    // INITIALIZERS
    
    Window(uint16_t width, uint16_t height, const char* name, Uint32 SDLflags = SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);
	~Window() { SDL_DestroyWindow(SDLHandle); }

    void commitSurface (Surface* surface, Instance* instance);

    // EXECUTION FUNCTIONS

    bool isWindowOpen();
    void processEvents();

    // GETTERS
    
    WindowSize getSize() const { return size; }
    const SDL_Window* getSDLHandle() const { return SDLHandle; }
    std::vector<const char*> getRequiredExtensions();

private:
    WindowSize size;
    SDL_Window* SDLHandle;

    bool wasQuitCalled = false;
};
