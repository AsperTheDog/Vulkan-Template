//
// Created by SATAN on 16/04/2023.
//

#include "CommandBuffers.hpp"

#include "CommandPool.hpp"
#include "LogicalDevice.hpp"
#include "RenderPass.hpp"
#include "GraphicsPipeline.hpp"
#include "Swapchain.hpp"

void CommandBuffers::commit(CommandPool* cPool)
{
    this->cPool = cPool;

    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.commandPool = cPool->getVKHandle();
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = 1;
    
    commandBuffers = std::make_shared<vk::raii::CommandBuffers>(*cPool->getLogicalDevice()->getRaiiHandle(), allocInfo);
}

CommandBuffer CommandBuffers::getCommandBuffer(uint32_t index)
{
	return CommandBuffer(this, index);
}

CommandBuffer::CommandBuffer(CommandBuffers* bufferList, uint32_t idx)
{
    buffer = &bufferList->getRaiiHandle()->at(idx);
    this->bufferList = bufferList;
}

void CommandBuffer::reset()
{
    buffer->reset((vk::CommandBufferResetFlagBits) 0);
}

void CommandBuffer::record(RenderPass* renderPass)
{
    vk::CommandBufferBeginInfo beginInfo{};
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
    
    buffer->begin(beginInfo);
    renderPass->begin(buffer);
	    
	    buffer->bindPipeline(vk::PipelineBindPoint::eGraphics, **renderPass->getGraphicsPipeline()->getRaiiHandle());
	    
	    // set the viewport
	    vk::Viewport viewport;
	    viewport.x = 0.0f;
	    viewport.y = 0.0f;
	    viewport.width = (float) renderPass->getGraphicsPipeline()->getSwapchain()->getExtent().width;
	    viewport.height = (float) renderPass->getGraphicsPipeline()->getSwapchain()->getExtent().height;
	    viewport.minDepth = 0.0f;
	    viewport.maxDepth = 1.0f;
	    buffer->setViewport(0, viewport);
	    
	    // set the scissor
	    vk::Rect2D scissor;
	    scissor.offset = vk::Offset2D{0, 0};
	    scissor.extent = renderPass->getGraphicsPipeline()->getSwapchain()->getExtent();
	    buffer->setScissor(0, scissor);
	    
	    // draw the triangle
	    buffer->draw(3, 1, 0, 0);
    
    renderPass->end(buffer);
    buffer->end();
}
