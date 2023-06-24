#pragma once
#include <xstring>

#include "Shader.hpp"
#include "SimpleVk_core.hpp"

namespace svk {
	class Shader;

	class GraphicsPipeline
	{
	public:
		~GraphicsPipeline();

		void commit(LogicalDevice* lDevice, bool optimizeShaders);

		void addShader(VkShaderStageFlagBits stage, const char* name, const Shader& shader);

	private:
		struct PipelineStage
		{
			VkShaderStageFlagBits stageInfo;
			std::string name;
			Shader shader;

			shaderc_shader_kind getKindFromStage();
		};

		VkPipeline vkHandle = nullptr;

		std::vector<PipelineStage> shaderStages{};

		LogicalDevice* logicalDevice = nullptr;
	};
}
