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
    
    Window(uint16_t width, uint16_t height, const char* name);
	~Window() { SDL_DestroyWindow(window); }

    std::vector<const char*> getRequiredExtensions();
    bool isWindowOpen();

    void processEvents();
    
    WindowSize getSize() const { return size; }

    const SDL_Window* getSDLHandle() const { return window; }

    void commitSurface (Surface* surface, Instance* instance);

private:
    WindowSize size;
    SDL_Window* window;

    bool wasQuitCalled = false;
};
