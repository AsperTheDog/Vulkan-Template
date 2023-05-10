//
// Created by SATAN on 09/04/2023.
//
#pragma once


#include <memory>
#include <unordered_set>

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

    void setAppVersion(uint32_t version) ;
    void setAppInfo(const std::string& appName, uint32_t appVersion, const std::string& engineName, uint32_t engineVersion);

    void addExtension(std::string extension) { extensions.push_back(std::move(extension)); }
    void addLayer(std::string layer) { layers.push_back(std::move(layer)); }

private:
    uint32_t version = VK_API_VERSION_1_0;
    vk::ApplicationInfo appInfo{
		"Vulkan App",
    	VK_MAKE_VERSION(1, 0, 0),
    	"No engine",
    	VK_MAKE_VERSION(1, 0, 0),
    	version
    };

    vk::raii::Context context{};
    std::shared_ptr<vk::raii::Instance> instance;
    
    static bool checkLayers(
        std::vector<char const *> const &layers, 
        std::vector<vk::LayerProperties> const &properties
    );

    std::vector<std::string> layers{};
    std::vector<std::string> extensions{};
};
