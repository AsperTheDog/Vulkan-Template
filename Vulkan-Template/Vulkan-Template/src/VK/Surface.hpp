//
// Created by SATAN on 09/04/2023.
//
#pragma once


#include <iostream>
#include <vulkan/vulkan_raii.hpp>

class Window;
class Instance;

class Surface
{
public:
    Surface() = default;

    void commit(Window* window, VkSurfaceKHR vkSurface, Instance* instance);
    
    std::shared_ptr<vk::raii::SurfaceKHR> getRaiiHandle() const { return surface; }
    VkSurfaceKHR getVKHandle() const { return **surface; }

    Window* getWindow() { return window; }
    Instance* getInstance() { return instance; }

private:
    Window* window{};
    Instance* instance{};

    std::shared_ptr<vk::raii::SurfaceKHR> surface;
};
