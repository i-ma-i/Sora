#pragma once
#include "Asset.hpp"

namespace sora
{
	enum class ShaderStage : uint8_t
	{
		Vertex,
		Pixel,
	};

	namespace ShaderLoader
	{
		constexpr static const char* StageToTarget(const ShaderStage stage) noexcept
		{
			switch (stage)
			{
			case ShaderStage::Vertex:
				return "vs_5_0";
			case ShaderStage::Pixel:
				return "ps_5_0";
			default:
				DebugBreak();
				return "";
			}
		};

		static ComPtr<ID3DBlob> LoadCSO(std::filesystem::path path)
		{
			LOG_INFO("Read shader from cso.  path[{}]", path.make_preferred().string());

			ComPtr<ID3DBlob> binary;
			if (FAILED(D3DReadFileToBlob(path.c_str(), &binary)))
			{
				DebugBreak();
			}
			return binary;
		}

		static ComPtr<ID3DBlob> CompileHLSLFromFile(const std::filesystem::path& path, const ShaderStage stage, const std::string_view entryPoint)
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
				DebugBreak();
			}
			if (FAILED(hr))
			{
				DebugBreak();
			}

			return binary;
		}
	}

	class VertexShader : public Asset
	{
	public:
		VertexShader() = default;
		VertexShader(const std::filesystem::path& path, Graphics* graphics, const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements)
			: Asset(path)
			, m_graphics(graphics)
			, m_inputElementDescs(inputElementDescs)
			, m_numElements(numElements)
		{}

		void LoadToMemory() override
		{
			const auto device = m_graphics->GetDevice();

			ComPtr<ID3DBlob> blob = ShaderLoader::LoadCSO(m_path);
			auto hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_vertexShader.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create vertex shader. PATH: {}", m_path.string());
				__debugbreak();
			}

			hr = device->CreateInputLayout(m_inputElementDescs, m_numElements, blob->GetBufferPointer(), blob->GetBufferSize(), m_inputLayout.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create input layout. PATH: {}", m_path.string());
				__debugbreak();
			}

			m_loadedInMemory = true;
		}

		void SetPipeline() const
		{
			const auto context = m_graphics->GetContext();
			context->IASetInputLayout(m_inputLayout.Get());
			context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		}

	private:
		Graphics* m_graphics = nullptr;
		const D3D11_INPUT_ELEMENT_DESC* m_inputElementDescs = nullptr;
		UINT m_numElements = 0;
		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11VertexShader> m_vertexShader;
	};

	class PixelShader : public Asset
	{
	public:
		PixelShader() = default;
		PixelShader(const std::filesystem::path& path, Graphics* graphics)
			: Asset(path)
			, m_graphics(graphics)
		{}

		void LoadToMemory() override
		{
			const auto device = m_graphics->GetDevice();


			ComPtr<ID3DBlob> blob = ShaderLoader::LoadCSO(m_path);
			auto hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pixelShader.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create pixel shader. PATH: {}", m_path.string());
				__debugbreak();
			}

			m_loadedInMemory = true;
		}

		void SetPipeline() const
		{
			const auto context = m_graphics->GetContext();
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		}

	private:
		Graphics* m_graphics = nullptr;
		ComPtr<ID3D11PixelShader> m_pixelShader;
	};
}
