//
// Created by SATAN on 16/04/2023.
//
#pragma once


#include <vulkan/vulkan_raii.hpp>

class CommandPool;
class CommandBuffers;
class RenderPass;

class CommandBuffer
{
public:
    CommandBuffer(CommandBuffers* bufferList, uint32_t idx);

    vk::raii::CommandBuffer* getVKRaiiHandle() { return commandBuffer; }
    VkCommandBuffer getVKBaseHandle() { return **commandBuffer; }

    void reset();
    void record(RenderPass* renderPass);

private:
    CommandBuffers* bufferList;
    vk::raii::CommandBuffer* commandBuffer;
};

class CommandBuffers
{
public:
    CommandBuffers() = default;

    void commit(CommandPool* cPool);

    CommandBuffer operator[](uint32_t index) { return getCommandBuffer(index); }
    
    std::shared_ptr<vk::raii::CommandBuffers> getVKRaiiHandle() const { return commandBuffers; }
    CommandBuffer getCommandBuffer(uint32_t index);

private:
    CommandPool* commandPool = nullptr;
    
    std::shared_ptr<vk::raii::CommandBuffers> commandBuffers;
};
