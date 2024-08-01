#pragma once
#include "AssetRegistry.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace sora
{
	class Quad
	{
	public:
		Quad(Graphics* graphics)
			: m_graphics(graphics)
		{
			using namespace DirectX;

			// 頂点データを作成する。
			const std::vector<VertexPositionNormalTexture> vertices =
			{
				{ XMFLOAT3{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f} },
				{ XMFLOAT3{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f} },
				{ XMFLOAT3{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f} },
				{ XMFLOAT3{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f} },
			};

			// インデックスデータを作成する。
			const UINT16 indices[] =
			{
				0, 1, 2, 0, 2, 3
			};

			const auto device = graphics->GetDevice();

			m_vertexBuffer = std::make_unique<VertexBuffer<VertexPositionNormalTexture>>(graphics, vertices);
			m_indexBuffer = std::make_unique<IndexBuffer16>(graphics, (void*)indices, ARRAYSIZE(indices));
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
		Graphics* m_graphics = nullptr;
	};
}
