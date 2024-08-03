#pragma once
#include "AssetRegistry.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace sora
{
	class Sphere
	{
	public:
		Sphere(Graphics* graphics)
			: m_graphics(graphics)
		{
			using namespace DirectX;
			using namespace DirectX::SimpleMath;

			constexpr UINT slices = 25;
			constexpr UINT stacks = 25;

			std::vector<VertexPositionNormalTexture> vertices;
			std::vector<UINT16> indices;
			vertices.reserve((slices + 1) * (stacks + 1));
			indices.reserve(slices * stacks * 6);

			// 球の頂点データを作成する。
			for (UINT i = 0; i <= stacks; i++)
			{
				float v = 1.0f - static_cast<float>(i) / stacks;
				float phi = v * XM_PI;

				for (UINT j = 0; j <= slices; j++)
				{
					float u = static_cast<float>(j) / slices;
					float theta = u * XM_2PI;

					float x = sinf(phi) * cosf(theta);
					float y = cosf(phi);
					float z = sinf(phi) * sinf(theta);

					Vector3 normal{ x, y, z };
					normal.Normalize();
					Vector2 texCoord{ u, v };

					vertices.emplace_back(Vector3(x, y, z) * 0.5f, normal, texCoord);
				}
			}
			m_vertexBuffer = std::make_unique<VertexBuffer<VertexPositionNormalTexture>>(graphics, vertices);

			// 球のインデックスデータを作成する。
			for (UINT i = 0; i < stacks; i++)
			{
				for (UINT j = 0; j < slices; j++)
				{
					indices.push_back((i + 1) * (slices + 1) + j);
					indices.push_back(i * (slices + 1) + j + 1);
					indices.push_back(i * (slices + 1) + j);

					indices.push_back((i + 1) * (slices + 1) + j);
					indices.push_back((i + 1) * (slices + 1) + j + 1);
					indices.push_back(i * (slices + 1) + j + 1);
				}
			}
			m_indexBuffer = std::make_unique<IndexBuffer16>(graphics, indices.data(), indices.size());

			std::filesystem::path filepath = "C:/dev/Sora/Sora/asset/texture/ground.png";
			HRESULT hr = CreateWICTextureFromFile(m_graphics->GetDevice(), filepath.wstring().c_str(), nullptr, m_texture.GetAddressOf());
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
