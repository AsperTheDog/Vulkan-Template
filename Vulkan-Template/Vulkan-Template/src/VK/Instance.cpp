//
// Created by SATAN on 09/04/2023.
//

#include "Instance.hpp"

#include "../SDL/Window.hpp"
#include "PhysicalDevice.hpp"

PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
	VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pMessenger)
{
	return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
	VkInstance instance, 
	VkDebugUtilsMessengerEXT messenger, 
	VkAllocationCallbacks const* pAllocator)
{
	return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageTypes,
	VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
	void* /*pUserData*/)
{
	std::cout << "awdkawpdokapd" << std::endl;
	return false;
	std::string message;

	message += vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) + ": " +
		vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) + ":\n";
	message += std::string("\t") + "messageIDName   = <" + pCallbackData->pMessageIdName + ">\n";
	message += std::string("\t") + "messageIdNumber = " + std::to_string(pCallbackData->messageIdNumber) + "\n";
	message += std::string("\t") + "message         = <" + pCallbackData->pMessage + ">\n";
	if (0 < pCallbackData->queueLabelCount)
	{
		message += std::string("\t") + "Queue Labels:\n";
		for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++)
		{
			message += std::string("\t\t") + "labelName = <" + pCallbackData->pQueueLabels[i].pLabelName + ">\n";
		}
	}
	if (0 < pCallbackData->cmdBufLabelCount)
	{
		message += std::string("\t") + "CommandBuffer Labels:\n";
		for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
		{
			message += std::string("\t\t") + "labelName = <" + pCallbackData->pCmdBufLabels[i].pLabelName + ">\n";
		}
	}
	if (0 < pCallbackData->objectCount)
	{
		for (uint32_t i = 0; i < pCallbackData->objectCount; i++)
		{
			message += std::string("\t") + "Object " + std::to_string(i) + "\n";
			message += std::string("\t\t") + "objectType   = " + vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)) + "\n";
			message += std::string("\t\t") + "objectHandle = " + std::to_string(pCallbackData->pObjects[i].objectHandle) + "\n";
			if (pCallbackData->pObjects[i].pObjectName)
			{
				message += std::string("\t\t") + "objectName   = <" + pCallbackData->pObjects[i].pObjectName + ">\n";
			}
		}
	}
	std::cout << message << std::endl;
	return false;
}


void mergeLists(std::vector<std::string>& from, std::vector<const char*>& to)
{
	for (auto& ext : from)
	{
		bool found = false;
		for (auto req : to)
		{
			if (strcmp(ext.c_str(), req) == 0)
				found = true;
		}
		if (!found)
			to.push_back(ext.c_str());
	}
}

void Instance::commit(Window* window)
{
	// Get the vkRaiiHandle layers
	std::vector<vk::LayerProperties> instanceLayerProperties = vkRaiiContext.enumerateInstanceLayerProperties();

	std::vector<char const*> instanceLayerNames;
	mergeLists(this->layers, instanceLayerNames);

	if (!checkLayers(instanceLayerNames, instanceLayerProperties))
		throw std::runtime_error("Set the environment variable VK_LAYER_PATH to point to the location of your layers");

	// Get the required extensions
	std::vector<const char*> requiredExtensions = window->getRequiredExtensions();
	mergeLists(this->extensions, requiredExtensions);

	// Create the vkRaiiHandle
	vk::InstanceCreateInfo instanceCreateInfo({}, &appInfo, instanceLayerNames, requiredExtensions);
	this->vkRaiiHandle = std::make_shared<vk::raii::Instance>(vkRaiiContext, instanceCreateInfo);

	if (areValidationLayersEnabled) setupDebugCallback();
}

void Instance::setupDebugCallback()
{
	pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>( getVKRaiiHandle()->getProcAddr( "vkCreateDebugUtilsMessengerEXT" ) );
    if ( !pfnVkCreateDebugUtilsMessengerEXT )
    {
      throw std::runtime_error("GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function.");
    }

    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>( getVKRaiiHandle()->getProcAddr( "vkDestroyDebugUtilsMessengerEXT" ) );
    if ( !pfnVkDestroyDebugUtilsMessengerEXT )
    {
      throw std::runtime_error("GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function.");
    }

	vk::DebugUtilsMessageSeverityFlagsEXT severityFlags( 
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning 
		| vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);

    vk::DebugUtilsMessageTypeFlagsEXT    messageTypeFlags( 
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral 
		| vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance 
		| vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

    vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT( {}, severityFlags, messageTypeFlags, &debugMessageFunc );
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( *getVKRaiiHandle(), debugUtilsMessengerCreateInfoEXT );
}

void Instance::enableValidationLayers()
{
	addLayer("VK_LAYER_KHRONOS_validation");
	addExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	areValidationLayersEnabled = true;
}

std::vector<PhysicalDevice> Instance::getPhysicalDevices()
{
	vk::raii::PhysicalDevices physicalDevices{ *this->getVKRaiiHandle() };
	std::vector<PhysicalDevice> pDevices;
	for (auto& pDevice : physicalDevices)
	{
		pDevices.emplace_back(pDevice, this);
	}
	return pDevices;
}

uint32_t Instance::getVersion()
{
	return vkRaiiContext.enumerateInstanceVersion();
}

void Instance::setAppVersion(uint32_t version)
{
	this->appInfo.apiVersion = version;
	this->version = version;
}

void Instance::setAppInfo(
	const std::string& appName, uint32_t appVersion,
	const std::string& engineName, uint32_t engineVersion)
{
	vk::ApplicationInfo appInfo{
		appName.c_str(), appVersion,
			engineName.c_str(), engineVersion,
			version
	};
	this->appInfo = appInfo;
}


bool Instance::checkLayers(std::vector<char const*> const& layers, std::vector<vk::LayerProperties> const& properties)
{
	return std::ranges::all_of(layers,
		[&properties](char const* name)
		{
			return std::ranges::find_if(properties,
			[&name](vk::LayerProperties const& property)
				{
					return strcmp(property.layerName, name) == 0;
				}) != properties.end();
		});
}
