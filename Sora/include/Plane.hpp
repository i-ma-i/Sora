#pragma once
#include "AssetRegistry.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace sora
{
	class Plane
	{
	public:
		Plane(Graphics* graphics)
			: m_graphics(graphics)
		{
			using namespace DirectX;
			using namespace DirectX::SimpleMath;

			std::vector<VertexPositionNormalTexture> vertices;
			std::vector<UINT16> indices;

			const int width = 11;
			const int height = 11;
			const float halfWidth = (width - 1) / 2.0f;
			const float halfHeight = (height - 1) / 2.0f;
			const float dx = 1.0f / (width - 1);
			const float dz = 1.0f / (height - 1);

			// 頂点データを作成する。
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					vertices.emplace_back(XMFLOAT3(-halfWidth + j, 0.0f, -halfHeight + i), Vector3::Up, XMFLOAT2(j * dx, i * dz));
				}
			}
			m_vertexBuffer = std::make_unique<VertexBuffer<VertexPositionNormalTexture>>(graphics, vertices);

			// インデックスデータを作成する。
			for (int i = 0; i < height - 1; ++i)
			{
				for (int j = 0; j < width - 1; ++j)
				{
					indices.push_back(i * width + j);
					indices.push_back((i + 1) * width + j);
					indices.push_back((i + 1) * width + (j + 1));

					indices.push_back(i * width + j);
					indices.push_back((i + 1) * width + (j + 1));
					indices.push_back(i * width + (j + 1));
				}
			}
			m_indexBuffer = std::make_unique<IndexBuffer16>(graphics, indices.data(), indices.size());
		}

		void Draw() const
		{
			m_vertexBuffer->SetPipeline();
			m_indexBuffer->SetPipeline();

			AssetRegistry::GetAsset<VertexShader>(VS)->SetPipeline();
			AssetRegistry::GetAsset<PixelShader>(PS)->SetPipeline();

			m_graphics->GetContext()->DrawIndexed(m_indexBuffer->GetIndexCount(), 0, 0);
		}

	private:
		static constexpr std::string_view VS = "Shader.Basic.VS";
		static constexpr std::string_view PS = "Shader.Basic.PS";
		std::unique_ptr<VertexBuffer<DirectX::VertexPositionNormalTexture>> m_vertexBuffer;
		std::unique_ptr<IndexBuffer16> m_indexBuffer;
		Graphics* m_graphics;
	};
}
