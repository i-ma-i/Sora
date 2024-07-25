#pragma once

namespace sora
{
	class Quad
	{
	public:
		Quad(const ComPtr<ID3D11Device>& device)
		{
			using namespace DirectX::SimpleMath;
			// 頂点データを作成する。
			DirectX::VertexPositionNormalTexture vertices[] =
			{
				{ Vector3(-10.0f, -1.0f, -10.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f) },
				{ Vector3( 10.0f, -1.0f, -10.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f) },
				{ Vector3( 10.0f, -1.0f,  10.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f) },
				{ Vector3(-10.0f, -1.0f,  10.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) },
			};
			m_stride = sizeof(vertices[0]);

			// 頂点バッファの作成
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(vertices);
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA initData = {};
			initData.pSysMem = vertices;

			HRESULT hr = device->CreateBuffer(&bd, &initData, m_vertexBuffer.ReleaseAndGetAddressOf());
			if (FAILED(hr))
				LOG_ERROR("Failed to create vertex buffer. HRESULT: {:#X}", hr);

			// インデックスデータを作成する。
			UINT indices[] =
			{
				0, 2, 1, 0, 3, 2
			};
			m_indexCount = ARRAYSIZE(indices);

			D3D11_BUFFER_DESC ibDesc = {};
			ibDesc.Usage = D3D11_USAGE_DEFAULT;
			ibDesc.ByteWidth = sizeof(indices);
			ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			ibDesc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA idata = {};
			idata.pSysMem = indices;

			hr = device->CreateBuffer(&ibDesc, &idata, m_indexBuffer.ReleaseAndGetAddressOf());
			if (FAILED(hr))
				LOG_ERROR("Failed to create index buffer. HRESULT: {:#X}", hr);
		}

		void Draw(const ComPtr<ID3D11DeviceContext>& context)
		{
			context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &m_offset);
			context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			context->DrawIndexed(m_indexCount, 0, 0);
		}

	private:
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		UINT m_stride = 0;
		UINT m_offset = 0;
		UINT m_indexCount = 0;
	};
}
