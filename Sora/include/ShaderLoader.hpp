#pragma once

namespace sora
{
	enum class ShaderStage : uint8_t
	{
		Vertex,
		Pixel
	};

	namespace ShaderLoader
	{
		ComPtr<ID3DBlob> LoadCSO(std::filesystem::path path);
		ComPtr<ID3DBlob> CompileHLSLFromFile(const std::filesystem::path& path, const ShaderStage stage, const std::string_view entryPoint);
	}
}
