#pragma once
#include "Asset.hpp"
#include "ShaderLoader.hpp"

namespace sora
{
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
}
