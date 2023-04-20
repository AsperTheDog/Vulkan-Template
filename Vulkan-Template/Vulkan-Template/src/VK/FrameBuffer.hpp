//
// Created by SATAN on 16/04/2023.
//
#pragma once


#include <vulkan/vulkan_raii.hpp>

class RenderPass;

class FrameBuffer
{
public:
    FrameBuffer() = default;

    void commit(vk::raii::ImageView& imgView, RenderPass* renderPass);
    
    std::shared_ptr<vk::raii::Framebuffer> getRaiiHandle() const { return frameBuffer; }
    VkFramebuffer getVKHandle() const { return **frameBuffer; }

    vk::Extent2D getSize() { return size; }

private:
    vk::Extent2D size{};
    std::shared_ptr<vk::raii::Framebuffer> frameBuffer;
};
