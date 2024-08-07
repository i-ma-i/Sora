#pragma once

namespace sora
{
	class Texture2D
	{
	public:
		Texture2D(std::filesystem::path path, Graphics* graphics)
			: m_graphics(graphics)
		{
			path.make_preferred();

			const auto device = m_graphics->GetDevice();

			ComPtr<ID3D11Resource> resource;
			auto hr = DirectX::CreateWICTextureFromFile(device, path.c_str(), resource.GetAddressOf(), m_texture.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create texture2D. PATH: {}", path.string());
				__debugbreak();
			}
		}

		void SetPipeline(UINT slot) const
		{
			m_graphics->GetContext()->PSSetShaderResources(slot, 1, m_texture.GetAddressOf());
		}

	private:
		Graphics* m_graphics = nullptr;
		ComPtr<ID3D11ShaderResourceView> m_texture;
	};
}
