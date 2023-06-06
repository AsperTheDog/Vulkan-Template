//
// Created by SATAN on 16/04/2023.
//

#include "FrameBuffer.hpp"

#include "RenderPass.hpp"
#include "GraphicsPipeline.hpp"
#include "Swapchain.hpp"
#include "LogicalDevice.hpp"


void FrameBuffer::commit(vk::raii::ImageView& imgView, RenderPass* renderPass)
{
    vk::FramebufferCreateInfo framebufferInfo{};
    framebufferInfo.renderPass = renderPass->getVKHandle();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &*imgView;
    framebufferInfo.width = renderPass->getGraphicsPipeline()->getSwapchain()->getExtent().width;
    framebufferInfo.height = renderPass->getGraphicsPipeline()->getSwapchain()->getExtent().height;
    framebufferInfo.layers = 1;
    
    size = vk::Extent2D(framebufferInfo.width, framebufferInfo.height);
    frameBuffer = std::make_shared<vk::raii::Framebuffer>(*renderPass->getGraphicsPipeline()->getSwapchain()->getLogicalDevice()->getVKRaiiHandle(), framebufferInfo);
}
