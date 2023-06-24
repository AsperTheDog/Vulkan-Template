#include "Shader.hpp"

#include <iostream>
#include <sstream>

#include "LogicalDevice.hpp"

namespace svk {
	Shader::Shader(const char* filename)
	{
		this->filename = filename;
	}

	Shader::~Shader()
	{
		if (logicalDevice == nullptr) return;
		vkDestroyShaderModule(logicalDevice->getVKHandle(), vkHandle, nullptr);
	}

	void Shader::commit(LogicalDevice* device, shaderc_shader_kind kind, bool optimize)
	{
		this->logicalDevice = device;
		auto shaderCode = readFile(filename);
		auto compiledCode = compileFile(filename, kind, shaderCode, optimize);
		createShaderModule(device, compiledCode);
	}

	std::string Shader::readFile(const std::string& filename)
	{
		std::ifstream shaderFile(filename);
		if (!shaderFile.is_open()) throw std::runtime_error("failed to open shader file");
		std::string str((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
		return str;
	}

	void Shader::createShaderModule(LogicalDevice* device, const std::vector<uint32_t>& code)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = 4 * code.size();
		createInfo.pCode = code.data();

		if (vkCreateShaderModule(device->getVKHandle(), &createInfo, nullptr, &this->vkHandle) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}
	}

	std::vector<uint32_t> Shader::compileFile(const std::string& source_name, shaderc_shader_kind kind,
		const std::string& source, bool optimize)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_performance);

		shaderc::SpvCompilationResult module =
			compiler.CompileGlslToSpv(source, kind, source_name.c_str(), options);

		if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
			std::cerr << module.GetErrorMessage();
			return {};
		}

		return { module.cbegin(), module.cend() };
	}
}
