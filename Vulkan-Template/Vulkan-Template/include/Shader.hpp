#pragma once
#include <string>
#include <fstream>
#include <vector>

#include "SimpleVk_core.hpp"
#include <shaderc/shaderc.hpp>

namespace svk {
	class LogicalDevice;

	class Shader
	{
	public:
		explicit Shader(const char* filename);
		~Shader();

		VkShaderModule getVKHandle() const { return vkHandle; }
		const std::string& getFilename() const { return filename; }

	private:
		void commit(LogicalDevice* device, shaderc_shader_kind kind, bool optimize);

		VkShaderModule vkHandle{};
		std::string filename;

		LogicalDevice* logicalDevice = nullptr;
		
		void createShaderModule(LogicalDevice* device, const std::vector<uint32_t>& code);

		static std::string readFile(const std::string& filename);
		static std::vector<uint32_t> compileFile(
			const std::string& source_name,
			shaderc_shader_kind kind,
			const std::string& source,
			bool optimize);

		friend class GraphicsPipeline;
	};
}
