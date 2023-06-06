//
// Created by SATAN on 09/04/2023.
//
#pragma once


#include <iostream>
#include <vulkan/vulkan_raii.hpp>

#include "PhysicalDevice.hpp"

class Window;
class Instance;

class Surface
{
public:
    Surface() = default;

    // INITIALIZERS

    void commit(Window* window, VkSurfaceKHR vkSurface, Instance* instance);

    // CONFIGURATION

    bool checkCompatibility(PhysicalDevice* pDevice);

    // GETTERS

    std::shared_ptr<vk::raii::SurfaceKHR> getVKRaiiHandle() const { return surface; }
    VkSurfaceKHR getVKBaseHandle() const { return **surface; }

    Window* getWindow() { return window; }
    Instance* getInstance() { return instance; }

private:
    Window* window{};
    Instance* instance{};

    std::shared_ptr<vk::raii::SurfaceKHR> surface;
};
