//
// Created by SATAN on 15/04/2023.
//

#include "RenderPass.hpp"
#include "GraphicsPipeline.hpp"
#include "LogicalDevice.hpp"
#include "Swapchain.hpp"

void RenderPass::addAttachment(vk::AttachmentDescription attachment)
{
    attachments.push_back(attachment);
}

void RenderPass::addSubPass(vk::SubpassDescription subpass)
{
    subpasses.push_back(subpass);
}

void RenderPass::commit(GraphicsPipeline *gPipeline, Swapchain* swapchain)
{
    this->gPipeline = gPipeline;
    
    // Configure render pass
    vk::RenderPassCreateInfo renderPassInfo{};
    renderPassInfo.attachmentCount = attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = subpasses.size();
    renderPassInfo.pSubpasses = subpasses.data();

    vk::SubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; 
    dependency.srcAccessMask = (vk::AccessFlagBits) 0;
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; 
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    renderPass = std::make_shared<vk::raii::RenderPass>(*swapchain->getLogicalDevice()->getVKRaiiHandle(), renderPassInfo);
}

void RenderPass::begin(vk::raii::CommandBuffer* cb)
{
    vk::RenderPassBeginInfo renderPassInfo{};
    renderPassInfo.renderPass = **renderPass;
    renderPassInfo.framebuffer = gPipeline->getSwapchain()->getActiveFramebuffer()->getVKHandle();
    renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
    renderPassInfo.renderArea.extent = gPipeline->getSwapchain()->getExtent();
    
    vk::ClearValue clearColor = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    cb->beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
}

void RenderPass::end(vk::raii::CommandBuffer *cBuffer)
{
    cBuffer->endRenderPass();
}
