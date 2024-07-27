﻿#pragma once
#include "ShaderLoader.hpp"

namespace sora
{
	class VertexShader
	{
	public:
		VertexShader(const ComPtr<ID3D11Device>& device, const std::filesystem::path& path)
		{
			ComPtr<ID3DBlob> blob = ShaderLoader::LoadCSO(path);

			// 頂点シェーダーを作成する。
			auto hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_vertexShader.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create vertex shader. HRESULT: {:#X}", hr);
				__debugbreak();
			}

			// インプットレイアウトを作成する。
			hr = device->CreateInputLayout(DirectX::VertexPositionNormalTexture::InputElements, DirectX::VertexPositionNormalTexture::InputElementCount, blob->GetBufferPointer(), blob->GetBufferSize(), m_inputLayout.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create input layout. HRESULT: {:#X}", hr);
				__debugbreak();
			}
		}

		void Bind(const ComPtr<ID3D11DeviceContext>& context) const
		{
			context->IASetInputLayout(m_inputLayout.Get());
			context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		}

	private:
		ComPtr<ID3D11VertexShader> m_vertexShader;
		ComPtr<ID3D11InputLayout> m_inputLayout;
	};
}