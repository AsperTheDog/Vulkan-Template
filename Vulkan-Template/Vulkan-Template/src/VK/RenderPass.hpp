//
// Created by SATAN on 15/04/2023.
//
#pragma once


#include <vulkan/vulkan_raii.hpp>

class Swapchain;
class GraphicsPipeline;

class RenderPass
{
public:
    RenderPass() = default;
    
    void addAttachment(vk::AttachmentDescription attachment);
    void addSubPass(vk::SubpassDescription subpass);

    void commit(GraphicsPipeline *gPipeline, Swapchain* swapchain);
    
    std::shared_ptr<vk::raii::RenderPass> getRaiiHandle() const { return renderPass; }
    VkRenderPass getVKHandle() const { return **renderPass; }
    
    GraphicsPipeline* getGraphicsPipeline() const { return gPipeline; }

    void begin(vk::raii::CommandBuffer* cb);
    void end(vk::raii::CommandBuffer *cBuffer);

private:
    std::vector<vk::AttachmentDescription> attachments;
    std::vector<vk::SubpassDescription> subpasses;

    GraphicsPipeline* gPipeline = nullptr;
    
    std::shared_ptr<vk::raii::RenderPass> renderPass;
};
