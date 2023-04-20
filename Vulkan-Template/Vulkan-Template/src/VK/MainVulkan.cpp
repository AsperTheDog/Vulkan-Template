//
// Created by SATAN on 09/04/2023.
//

#include "MainVulkan.hpp"

MainVulkan::MainVulkan() : window{ 1920, 1080, "Vulkan Template" }, pDevice{nullptr, nullptr}
{
	instance.commit(&window);

	if (findPhysicalDevice())
		throw std::runtime_error("No suitable physical device found");

	lDevice.commit(&pDevice);
	swapchain.commit(&lDevice, instance.getSurface());
	gPipeline.commit(&swapchain);
	cPool.commit(&lDevice);
	cBuffers.commit(&cPool);
	imageAvailableSemaphore.commit(&lDevice);
	renderFinishedSemaphore.commit(&lDevice);
	inFlightFence.commit(&lDevice);
}

void MainVulkan::run()
{
	while (window.isWindowOpen())
	{
		window.processEvents();
		drawFrame();
	}

	lDevice.waitIdle();
}

void MainVulkan::drawFrame()
{
	inFlightFence.wait();
	inFlightFence.reset();

	uint32_t nextImg = swapchain.acquireNextImage(&imageAvailableSemaphore);
	swapchain.setActiveIndex(nextImg);

	auto cmdBuffer = cBuffers.getCommandBuffer(0);
	cmdBuffer.reset();
	cmdBuffer.record(gPipeline.getRenderPass());

	vk::SubmitInfo submitInfo{};

    vk::Semaphore waitSemaphores[] = {**imageAvailableSemaphore.getRaiiHandle()};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
	auto vkBuffer = **cmdBuffer.getRaiiHandle();
    submitInfo.pCommandBuffers = &vkBuffer;

    vk::Semaphore signalSemaphores[] = {**renderFinishedSemaphore.getRaiiHandle()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

	lDevice.submitToQueue({submitInfo}, &inFlightFence);

	vk::PresentInfoKHR presentInfo{};

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = {**swapchain.getRaiiHandle()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &nextImg;

	lDevice.presentInQueue(&presentInfo);
}

bool MainVulkan::findPhysicalDevice()
{
	for (auto& pDevice : instance.getPhysicalDevices())
	{
		if (pDevice.isSuitable())
		{
			this->pDevice = pDevice;
			break;
		}
	}

	return this->pDevice.getRaiiHandle() == nullptr;

}
