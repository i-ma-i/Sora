#include "VertexShader.hpp"
#include "ShaderLoader.hpp"

namespace sora
{
	VertexShader::VertexShader(const std::filesystem::path& path, const ComPtr<ID3D11Device>& device, const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements)
	{
		ComPtr<ID3DBlob> blob = ShaderLoader::LoadCSO(path);

		// 頂点シェーダーを作成する。
		auto hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_vertexShader.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			LOG_ERROR("Failed to create vertex shader. HRESULT: {:#X}", hr);
			__debugbreak();
		}

		hr = device->CreateInputLayout(
			pInputElementDescs,
			NumElements,
			blob->GetBufferPointer(), blob->GetBufferSize(),
			m_inputLayout.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			LOG_ERROR("Failed to create input layout. HRESULT: {:#X}", hr);
			__debugbreak();
		}
	}

	void VertexShader::Bind(const ComPtr<ID3D11DeviceContext>& context) const
	{
		context->IASetInputLayout(m_inputLayout.Get());
		context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	}
}
