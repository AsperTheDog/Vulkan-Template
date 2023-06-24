#include "GraphicsPipeline.hpp"

#include <utility>

#include "LogicalDevice.hpp"
#include "Shader.hpp"

namespace svk
{
	GraphicsPipeline::~GraphicsPipeline()
	{
		if (this->logicalDevice == nullptr) return;
		vkDestroyPipeline(this->logicalDevice->getVKHandle(), vkHandle, nullptr);
	}

	void GraphicsPipeline::commit(LogicalDevice* lDevice, bool optimizeShaders)
	{
		this->logicalDevice = lDevice;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos;
		for (auto& stageInfo : shaderStages)
		{
			stageInfo.shader.commit(this->logicalDevice, stageInfo.getKindFromStage(), optimizeShaders);

			VkPipelineShaderStageCreateInfo shaderStageInfo{};
			shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageInfo.stage = stageInfo.stageInfo;
			shaderStageInfo.module = stageInfo.shader.getVKHandle();
			shaderStageInfo.pName = stageInfo.name.c_str();

			shaderStageInfos.push_back(shaderStageInfo);
		}

		// TODO: Create the graphics pipeline

		shaderStages.clear();
	}

	void GraphicsPipeline::addShader(VkShaderStageFlagBits stage, const char* name, const Shader& shader)
	{
		PipelineStage stageInfo{stage, name, shader};
		shaderStages.push_back(stageInfo);
	}

	shaderc_shader_kind svk::GraphicsPipeline::PipelineStage::getKindFromStage()
	{
		switch (stageInfo)
		{
		case VK_SHADER_STAGE_VERTEX_BIT: return shaderc_vertex_shader;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return shaderc_tess_control_shader;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return shaderc_tess_evaluation_shader;
		case VK_SHADER_STAGE_GEOMETRY_BIT: return shaderc_geometry_shader;
		case VK_SHADER_STAGE_FRAGMENT_BIT: return shaderc_fragment_shader;
		case VK_SHADER_STAGE_COMPUTE_BIT: return shaderc_compute_shader;
		case VK_SHADER_STAGE_RAYGEN_BIT_KHR: return shaderc_raygen_shader;
		case VK_SHADER_STAGE_ANY_HIT_BIT_KHR: return shaderc_anyhit_shader;
		case VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR: return shaderc_closesthit_shader;
		case VK_SHADER_STAGE_MISS_BIT_KHR: return shaderc_miss_shader;
		case VK_SHADER_STAGE_INTERSECTION_BIT_KHR: return shaderc_intersection_shader;
		case VK_SHADER_STAGE_CALLABLE_BIT_KHR: return shaderc_callable_shader;
		case VK_SHADER_STAGE_TASK_BIT_EXT: return shaderc_task_shader;
		case VK_SHADER_STAGE_MESH_BIT_EXT: return shaderc_mesh_shader;
		default: throw std::runtime_error("Unsupported shader stage");
		}
	}
}
