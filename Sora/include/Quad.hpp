#pragma once
#include "AssetRegistry.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"

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
			const static VertexPositionNormalTexture vertices[] =
			{
				{ XMFLOAT3{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f} },
				{ XMFLOAT3{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f} },
				{ XMFLOAT3{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f} },
				{ XMFLOAT3{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f} },
			};
			m_stride = sizeof(vertices[0]);

			// インデックスデータを作成する。
			constexpr UINT16 indices[] =
			{
				0, 1, 2, 0, 2, 3
			};
			m_indexCount = ARRAYSIZE(indices);

			const auto device = graphics->GetDevice();

			// 頂点バッファを作成する。
			D3D11_BUFFER_DESC desc = {};
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.ByteWidth = sizeof(vertices);
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			D3D11_SUBRESOURCE_DATA vertexData = {};
			vertexData.pSysMem = vertices;
			HRESULT hr = device->CreateBuffer(&desc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf());
			if (FAILED(hr))
				LOG_ERROR("Failed to create vertex buffer. HRESULT: {:#X}", hr);

			// インデックスバッファを作成する。
			desc.ByteWidth = sizeof(indices);
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			D3D11_SUBRESOURCE_DATA indexData = {};
			indexData.pSysMem = indices;
			hr = device->CreateBuffer(&desc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf());
			if (FAILED(hr))
				LOG_ERROR("Failed to create index buffer. HRESULT: {:#X}", hr);
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
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		UINT m_stride = 0;
		UINT m_offset = 0;
		UINT m_indexCount = 0;
		Graphics* m_graphics = nullptr;
	};
}
