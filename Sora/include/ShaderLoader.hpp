#pragma once

namespace sora
{
	enum class ShaderStage : uint8_t
	{
		Vertex,
		Pixel
	};
	constexpr const char* StageToTarget(const ShaderStage stage) noexcept
	{
		switch (stage)
		{
		case ShaderStage::Vertex:
			return "vs_5_0";
		case ShaderStage::Pixel:
			return "ps_5_0";
		default:
			__debugbreak();
			return "";
		}
	}

	namespace ShaderLoader
	{
		ComPtr<ID3DBlob> LoadCSO(const std::filesystem::path& path)
		{
			LOG_INFO("Read shader from cso.  path[{}]", path.string());

			ComPtr<ID3DBlob> binary;
			if (FAILED(D3DReadFileToBlob(path.c_str(), &binary)))
			{
				__debugbreak();
			}
			return binary;
		}

		ComPtr<ID3DBlob> CompileHLSLFromFile(const std::filesystem::path& path, const ShaderStage stage, const std::string_view entryPoint)
		{
			LOG_INFO("Compile shader from HLSL.  path[{}]", path.string());

			DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
			dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

			ComPtr<ID3DBlob> binary, error;
			const HRESULT hr = D3DCompileFromFile(path.c_str(), nullptr, nullptr, entryPoint.data(), StageToTarget(stage), dwShaderFlags, 0, &binary, &error);

			if (error)
			{
				__debugbreak();
			}
			if (FAILED(hr))
			{
				__debugbreak();
			}

			return binary;
		}
	}
}
