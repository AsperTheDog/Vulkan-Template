#include "Semaphore.hpp"

#include "../LogicalDevice.hpp"

void Semaphore::commit(LogicalDevice* lDevice)
{
	vk::SemaphoreCreateInfo semaphoreInfo{};
	
	semaphore = std::make_shared<vk::raii::Semaphore>(*lDevice->getVKRaiiHandle(), semaphoreInfo);
}
