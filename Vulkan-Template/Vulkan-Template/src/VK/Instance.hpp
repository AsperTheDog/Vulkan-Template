//
// Created by SATAN on 09/04/2023.
//
#pragma once


#include <memory>

#include "Surface.hpp"

class Window;
class PhysicalDevice;

class Instance
{
public:
    Instance() = default;

    void commit(Window* window);
    
    std::shared_ptr<vk::raii::Instance> getRaiiHandle() const { return instance; }
    VkInstance getVKHandle() const { return **instance; }

    std::vector<PhysicalDevice> getPhysicalDevices();

    uint32_t getVersion();
    Surface* getSurface() { return &surface; }

private:
    vk::raii::Context context{};
    std::shared_ptr<vk::raii::Instance> instance;
    Surface surface;
    
    static bool checkLayers(
        std::vector<char const *> const &layers, 
        std::vector<vk::LayerProperties> const &properties
    );
};
