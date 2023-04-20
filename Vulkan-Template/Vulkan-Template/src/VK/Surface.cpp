//
// Created by SATAN on 09/04/2023.
//

#include "Surface.hpp"
#include "Instance.hpp"

void Surface::commit(Window* window, VkSurfaceKHR vkSurface, Instance* instance)
{
    this->window = window;
    this->instance = instance;
    this->surface = std::make_shared<vk::raii::SurfaceKHR>(*instance->getRaiiHandle(), vkSurface);
}
