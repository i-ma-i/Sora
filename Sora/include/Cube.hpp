#pragma once
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace sora
{
	class Cube
	{
	public:
		Cube(Graphics* graphics)
			: m_graphics(graphics)
		{
			using namespace DirectX;

			// 頂点データを作成する。
			const std::vector<VertexPositionNormalTexture> vertices =
			{
				{ XMFLOAT3{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.75f, 2.0f / 3.0f} },// Front
				{ XMFLOAT3{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.75f, 1.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.00f, 1.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.00f, 2.0f / 3.0f} },

				{ XMFLOAT3{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.25f, 2.0f / 3.0f} },// Back
				{ XMFLOAT3{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.25f, 1.0f / 3.0f} },
				{ XMFLOAT3{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.50f, 1.0f / 3.0f} },
				{ XMFLOAT3{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.50f, 2.0f / 3.0f} },

				{ XMFLOAT3{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.25f, 1.0f / 3.0f} },// Top
				{ XMFLOAT3{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.25f, 0.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.50f, 0.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.50f, 1.0f / 3.0f} },

				{ XMFLOAT3{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.25f, 3.0f / 3.0f} },// Bottom
				{ XMFLOAT3{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.25f, 2.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.50f, 2.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.50f, 3.0f / 3.0f} },

				{ XMFLOAT3{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.00f, 2.0f / 3.0f} },// Left
				{ XMFLOAT3{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.00f, 1.0f / 3.0f} },
				{ XMFLOAT3{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.25f, 1.0f / 3.0f} },
				{ XMFLOAT3{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.25f, 2.0f / 3.0f} },

				{ XMFLOAT3{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.50f, 2.0f / 3.0f} },// Right
				{ XMFLOAT3{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.50f, 1.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.75f, 1.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.75f, 2.0f / 3.0f} },
			};
			m_vertexBuffer = std::make_unique<VertexBuffer<VertexPositionNormalTexture>>(graphics, vertices);

			// インデックスデータを作成する。
			constexpr UINT16 indices[] =
			{
				0, 1, 2, 0, 2, 3,// Front
				4, 5, 6, 4, 6, 7,// Back
				8, 9, 10, 8, 10, 11,// Top
				12, 13, 14, 12, 14, 15,// Bottom
				16, 17, 18, 16, 18, 19,// Left
				20, 21, 22, 20, 22, 23,// Right
			};
			m_indexBuffer = std::make_unique<IndexBuffer16>(graphics, (void*)indices, ARRAYSIZE(indices));

			std::filesystem::path filepath = "C:/dev/Sora/Sora/asset/texture/cubemap.bmp";
			HRESULT hr = CreateWICTextureFromFile(graphics->GetDevice(), filepath.wstring().c_str(), nullptr, m_texture.GetAddressOf()
			);
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to load texture.  path[{}]", filepath.string());
				__debugbreak();
			}
			LOG_DEBUG("Texture loaded. [path]{}", filepath.string());
		}

		void Draw() const
		{
			m_vertexBuffer->SetPipeline();
			m_indexBuffer->SetPipeline();

			AssetRegistry::GetAsset<VertexShader>(VS)->SetPipeline();
			AssetRegistry::GetAsset<PixelShader>(PS)->SetPipeline();

			const auto context = m_graphics->GetContext();
			context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
			context->DrawIndexed(m_indexBuffer->GetIndexCount(), 0, 0);
		}

	private:
		static constexpr std::string_view VS = "Shader.Basic.VS";
		static constexpr std::string_view PS = "Shader.Basic.PS";
		std::unique_ptr<VertexBuffer<DirectX::VertexPositionNormalTexture>> m_vertexBuffer;
		std::unique_ptr<IndexBuffer16> m_indexBuffer;
		ComPtr<ID3D11ShaderResourceView> m_texture;
		Graphics* m_graphics = nullptr;
	};
}
