//
// Created by SATAN on 09/04/2023.
//
#pragma once
#include "../SDL/Window.hpp"
#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"
#include "Swapchain.hpp"
#include "GraphicsPipeline.hpp"
#include "CommandPool.hpp"
#include "CommandBuffers.hpp"
#include "Synchronization/Fence.hpp"
#include "Synchronization/Semaphore.hpp"

class MainVulkan {
public:
	MainVulkan();

	void run();
	void drawFrame();

private:
	Window window;
	Instance instance;
	Surface surface;
	PhysicalDevice pDevice;
	LogicalDevice lDevice;
	Swapchain swapchain;
	GraphicsPipeline gPipeline;
	CommandPool cPool;
	CommandBuffers cBuffers;
	Semaphore imageAvailableSemaphore;
	Semaphore renderFinishedSemaphore;
	Fence inFlightFence;

	bool findPhysicalDevice();
	static bool isPDeviceSuitable(const PhysicalDevice& device);
};
