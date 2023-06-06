//
// Created by SATAN on 09/04/2023.
//
#pragma once


#include <memory>
#include <sstream>

#include "Surface.hpp"

class Window;
class PhysicalDevice;

class Instance
{
public:
	// INITIALIZERS

	void commit(Window* window);
	void setupDebugCallback();

	// CONFIGURATION

	void enableValidationLayers();

	void addExtension(std::string extension) { extensions.push_back(std::move(extension)); }
	void addLayer(std::string layer) { layers.push_back(std::move(layer)); }

	void setAppVersion(uint32_t version);
	void setAppInfo(const std::string& appName, uint32_t appVersion, const std::string& engineName, uint32_t engineVersion);

	// GETTERS

	std::shared_ptr<vk::raii::Instance> getVKRaiiHandle() const { return vkRaiiHandle; }
	VkInstance getVKBaseHandle() const { return **vkRaiiHandle; }
	std::vector<PhysicalDevice> getPhysicalDevices();
	uint32_t getVersion();

private:
	bool areValidationLayersEnabled = false;

	uint32_t version = VK_API_VERSION_1_0;
	vk::ApplicationInfo appInfo{
		"Vulkan App",
			VK_MAKE_VERSION(1, 0, 0),
			"No engine",
			VK_MAKE_VERSION(1, 0, 0),
			version
	};

	vk::raii::Context vkRaiiContext{};
	std::shared_ptr<vk::raii::Instance> vkRaiiHandle;

	std::vector<std::string> layers{};
	std::vector<std::string> extensions{};

	static bool checkLayers(
		std::vector<char const*> const& layers,
		std::vector<vk::LayerProperties> const& properties
	);

	
};
