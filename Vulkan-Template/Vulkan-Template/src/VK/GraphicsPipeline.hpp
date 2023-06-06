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

    std::shared_ptr<vk::raii::Pipeline> getVKRaiiHandle() const { return graphicsPipeline; }
    VkPipeline getVKBaseHandle() const { return **graphicsPipeline; }

    Swapchain* getSwapchain() const { return swapchain; }
    RenderPass* getRenderPass();

private:
    Swapchain* swapchain;
    std::shared_ptr<vk::raii::Pipeline> graphicsPipeline{};

    GraphicsPipelineLayout graphicsPipelineLayout{};
    RenderPass renderPass;
    
    static std::vector<uint32_t> compile_file(
            const std::string &source_name,
            shaderc_shader_kind kind,
            const std::string &source,
            bool optimize = false);
    
    inline static const std::string fragmentShaderFilename = "shader.frag";
    inline static const std::string vertexShaderFilename = "shader.vert";
};
