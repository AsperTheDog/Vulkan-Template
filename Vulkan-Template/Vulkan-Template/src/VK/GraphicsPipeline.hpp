//
// Created by SATAN on 15/04/2023.
//
#pragma once


#include <vulkan/vulkan_raii.hpp>
#include <shaderc/shaderc.hpp>

#include "GraphicsPipelineLayout.hpp"
#include "RenderPass.hpp"

class Swapchain;
class GraphicsPipelineLayout;

class GraphicsPipeline
{
public:
    GraphicsPipeline() = default;

    void commit(Swapchain* swapchain);

    std::shared_ptr<vk::raii::Pipeline> getRaiiHandle() const { return graphicsPipeline; }
    VkPipeline getVKHandle() const { return **graphicsPipeline; }

    Swapchain* getSwapchain() const { return swapchain; }
    RenderPass* getRenderPass();

private:
    Swapchain* swapchain;
    std::shared_ptr<vk::raii::Pipeline> graphicsPipeline{};

    GraphicsPipelineLayout pipelineLayout{};
    RenderPass renderPass;
    
    static std::vector<uint32_t> compile_file(
            const std::string &source_name,
            shaderc_shader_kind kind,
            const std::string &source,
            bool optimize = false);
    
    inline static const std::string fragShader = "shader.frag";
    inline static const std::string vertShader = "shader.vert";
};
