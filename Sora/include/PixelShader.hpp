#pragma once
#include "Asset.hpp"
#include "ShaderLoader.hpp"

namespace sora
{
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
