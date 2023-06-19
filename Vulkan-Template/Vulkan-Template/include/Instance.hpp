#pragma once

#include <utility>
#include <vector>
#include <string>

#include "SimpleVk_core.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"

namespace svk {
	class Instance
	{
	public:
		~Instance();

		void commit(uint32_t apiVersion);

		void addValidationLayers();

		void addLayer(const char* layer);
		void addExtension(const char* extension);

		void setAppName(std::string name) { this->appName = std::move(name); }
		void setAppVersion(uint32_t version) { this->appVersion = version; }
		void setEngineName(std::string name) { this->engineName = std::move(name); }
		void setEngineVersion(uint32_t version) { this->engineVersion = version; }
		
		VkInstance getVkHandle() { return vkHandle; }
		uint32_t getApiVersion() const { return this->apiVersion; }
		std::string getAppName() const { return this->appName; }
		uint32_t getAppVersion() const { return this->appVersion; }
		std::string getEngineName() const { return this->engineName; }
		uint32_t getEngineVersion() const { return this->engineVersion; }
		std::vector<PhysicalDevice>* getPhysicalDevices();
		
	private:
		VkInstance vkHandle = nullptr;

		std::vector<const char*> layers{};
		std::vector<const char*> extensions{};

		uint32_t apiVersion = 0;

		std::string appName = "Vulkan Application";
		uint32_t appVersion = VK_MAKE_VERSION(1, 0, 0);
		std::string engineName = "No Engine";
		uint32_t engineVersion = VK_MAKE_VERSION(1, 0, 0);

		std::vector<PhysicalDevice> physicalDevices;
	};
}

