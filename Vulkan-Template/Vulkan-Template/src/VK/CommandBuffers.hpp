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

    vk::raii::CommandBuffer* getRaiiHandle() { return buffer; }
    VkCommandBuffer getVKHandle() { return **buffer; }

    void reset();
    void record(RenderPass* renderPass);

private:
    CommandBuffers* bufferList;
    vk::raii::CommandBuffer* buffer;
};

class CommandBuffers
{
public:
    CommandBuffers() = default;

    CommandBuffer operator[](uint32_t index) { return getCommandBuffer(index); }

    void commit(CommandPool* cPool);
    
    std::shared_ptr<vk::raii::CommandBuffers> getRaiiHandle() const { return commandBuffers; }

    CommandBuffer getCommandBuffer(uint32_t index);
private:
    CommandPool* cPool = nullptr;
    
    std::shared_ptr<vk::raii::CommandBuffers> commandBuffers;
};
