#pragma once
#include "ShaderLoader.hpp"

namespace sora
{
	class PixelShader
	{
	public:
		PixelShader(const ComPtr<ID3D11Device>& device, const std::filesystem::path& path)
		{
			ComPtr<ID3DBlob> blob = ShaderLoader::LoadCSO(path);

			auto hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pixelShader.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create pixel shader. HRESULT: {:#X}", hr);
				__debugbreak();
			}
		}

		void Bind(const ComPtr<ID3D11DeviceContext>& context) const
		{
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		}

	private:
		ComPtr<ID3D11PixelShader> m_pixelShader;
	};
}
