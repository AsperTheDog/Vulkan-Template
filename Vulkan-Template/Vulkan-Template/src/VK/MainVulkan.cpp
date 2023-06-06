//
// Created by SATAN on 09/04/2023.
//

#include "MainVulkan.hpp"

#include "QueueFamilies.hpp"

MainVulkan::MainVulkan() : window{ 1920, 1080, "Vulkan Template" }, physicalDevice{nullptr, nullptr}
{
		
#ifdef _DEBUG
	instance.enableValidationLayers();
#endif

	instance.setAppVersion(VK_API_VERSION_1_3);
	instance.commit(&window);

	window.commitSurface(&surface, &instance);

	if (!findPhysicalDevice())
		throw std::runtime_error("No suitable physical device found");

	QueueFamilies* queues = physicalDevice.getQueueFamilies();

	logicalDevice.setPhysicalDevice(&physicalDevice);
	for (auto& queue : *queues->getQueues())
		logicalDevice.setQueuePriorities(queue.queue.index, {1.0f});
	logicalDevice.commit();

	swapchain.setPreferredFormat({
		{vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}
	});
	swapchain.setPreferredPresentMode({
		vk::PresentModeKHR::eMailbox,
		vk::PresentModeKHR::eFifo,
		vk::PresentModeKHR::eImmediate
	});
	swapchain.commit(&logicalDevice, &surface);

	graphicsPipeline.commit(&swapchain);

	commandPool.commit(&logicalDevice, FamilyQueueGraphics, 0);

	commandBuffers.commit(&commandPool);

	imageAvailableSemaphore.commit(&logicalDevice);
	renderFinishedSemaphore.commit(&logicalDevice);

	inFlightFence.commit(&logicalDevice);
}

void MainVulkan::run()
{
	while (window.isWindowOpen())
	{
		window.processEvents();
		drawFrame();
	}

	logicalDevice.waitIdle();
}

void MainVulkan::drawFrame()
{
	inFlightFence.wait();
	inFlightFence.reset();

	uint32_t nextImg = swapchain.acquireNextImage(&imageAvailableSemaphore);
	swapchain.setActiveIndex(nextImg);

	auto cmdBuffer = commandBuffers.getCommandBuffer(0);
	cmdBuffer.reset();
	cmdBuffer.record(graphicsPipeline.getRenderPass());

	vk::SubmitInfo submitInfo{};

    vk::Semaphore waitSemaphores[] = {**imageAvailableSemaphore.getVKRaiiHandle()};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
	auto vkBuffer = **cmdBuffer.getVKRaiiHandle();
    submitInfo.pCommandBuffers = &vkBuffer;

    vk::Semaphore signalSemaphores[] = {**renderFinishedSemaphore.getVKRaiiHandle()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

	logicalDevice.submitToQueue(FamilyQueueGraphics, 0, {submitInfo}, &inFlightFence);

	vk::PresentInfoKHR presentInfo{};

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = {**swapchain.getVKRaiiHandle()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &nextImg;

	logicalDevice.presentInQueue(0, &presentInfo);
}

bool MainVulkan::findPhysicalDevice()
{
	bool found = false;
	for (auto& physicalDevice : instance.getPhysicalDevices())
	{
		physicalDevice.setSurface(&surface);

		QueueFamilies queues{&physicalDevice};
		auto graphicsQueues = queues.findQueues(FamilyQueueGraphics);
		if (graphicsQueues.empty()) continue;
		auto presentQueues = queues.findQueues(FamilyQueuePresent);
		if (presentQueues.empty()) continue;
		queues.addQueue(graphicsQueues[0]);
		queues.addQueue(presentQueues[0]);
		physicalDevice.setQueuesToUse(queues);

		if (isPDeviceSuitable(physicalDevice))
		{
			this->physicalDevice = physicalDevice;
			found = true;
			break;
		}
	}
	return found;
}

bool MainVulkan::isPDeviceSuitable(PhysicalDevice& device)
{
	auto props = device.getProperties();
	
	if (props.apiVersion < instance.getVersion())
		return false;
	
	// Check the device is a discrete GPU
	return surface.checkCompatibility(&device) && props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
}
