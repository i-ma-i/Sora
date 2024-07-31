#pragma once
#include "AssetRegistry.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"

namespace sora
{
	class Plane
	{
	public:
		Plane(Graphics* graphics)
			: m_graphics(graphics)
		{
			using namespace DirectX;

			std::vector<VertexPositionNormalTexture> vertices;
			std::vector<UINT16> indices;

			const int width = 10;
			const int height = 10;
			const float halfWidth = width / 2.0f;
			const float halfHeight = height / 2.0f;
			const float dx = 1.0f / (width - 1);
			const float dz = 1.0f / (height - 1);

			// 頂点データを作成する。
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					VertexPositionNormalTexture vertex;
					vertex.position = XMFLOAT3(-halfWidth + j, 0.0f, -halfHeight + i);
					vertex.normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
					vertex.textureCoordinate = XMFLOAT2(j * dx, i * dz);
					vertices.push_back(vertex);
				}
			}
			m_stride = sizeof(VertexPositionNormalTexture);

			// インデックスデータを作成する。
			for (int i = 0; i < height - 1; i++)
			{
				for (int j = 0; j < width - 1; j++)
				{
					indices.push_back(i * width + j);
					indices.push_back((i + 1) * width + j);
					indices.push_back((i + 1) * width + (j + 1));

					indices.push_back(i * width + j);
					indices.push_back((i + 1) * width + (j + 1));
					indices.push_back(i * width + (j + 1));
				}
			}
			m_indexCount = static_cast<UINT>(indices.size());

			const auto device = graphics->GetDevice();

			// 頂点バッファを作成する。
			D3D11_BUFFER_DESC vertexBufferDesc{};
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = static_cast<UINT>(vertices.size() * m_stride);
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			D3D11_SUBRESOURCE_DATA vertexBufferData{};
			vertexBufferData.pSysMem = vertices.data();
			HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_vertexBuffer.GetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create vertex buffer. HRESULT: {:#X}", hr);
				__debugbreak();
			}

			// インデックスバッファを作成する。
			D3D11_BUFFER_DESC indexBufferDesc{};
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(UINT16) * m_indexCount);
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			D3D11_SUBRESOURCE_DATA indexBufferData{};
			indexBufferData.pSysMem = indices.data();
			hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create index buffer. HRESULT: {:#X}", hr);
				__debugbreak();
			}
		}

		void Draw() const
		{
			const auto context = m_graphics->GetContext();

			context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &m_offset);
			context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

			AssetRegistry::GetAsset<VertexShader>(VS)->SetPipeline();
			AssetRegistry::GetAsset<PixelShader>(PS)->SetPipeline();

			context->DrawIndexed(m_indexCount, 0, 0);
		}

	private:
		static constexpr std::string_view VS = "Shader.Basic.VS";
		static constexpr std::string_view PS = "Shader.Basic.PS";
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
		UINT m_stride = 0;
		UINT m_offset = 0;
		UINT m_indexCount = 0;
		Graphics* m_graphics;
	};
}
