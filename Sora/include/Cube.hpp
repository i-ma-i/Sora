#pragma once
#include "VertexShader.hpp"
#include "PixelShader.hpp"

namespace sora
{
	class Cube
	{
	public:
		Cube(Graphics* graphics)
			: m_graphics(graphics)
		{
			using namespace DirectX;
			using namespace DirectX::SimpleMath;

			const auto device = graphics->GetDevice();
			const auto context = graphics->GetContext();

			// 頂点データを作成する。
			const static VertexPositionNormalTexture vertices[] =
			{
				{ XMFLOAT3{-0.5f, -0.5f, -0.5f}, XMFLOAT3{0.0f, 0.0f, -1.0f}, XMFLOAT2{0.75f, 2.0f / 3.0f} },// Front
				{ XMFLOAT3{-0.5f,  0.5f, -0.5f}, XMFLOAT3{0.0f, 0.0f, -1.0f}, XMFLOAT2{0.75f, 1.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f,  0.5f, -0.5f}, XMFLOAT3{0.0f, 0.0f, -1.0f}, XMFLOAT2{1.00f, 1.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f, -0.5f, -0.5f}, XMFLOAT3{0.0f, 0.0f, -1.0f}, XMFLOAT2{1.00f, 2.0f / 3.0f} },

				{ XMFLOAT3{ 0.5f, -0.5f,  0.5f}, XMFLOAT3{0.0f, 0.0f, 1.0f}, XMFLOAT2{0.25f, 2.0f / 3.0f} },// Back
				{ XMFLOAT3{ 0.5f,  0.5f,  0.5f}, XMFLOAT3{0.0f, 0.0f, 1.0f}, XMFLOAT2{0.25f, 1.0f / 3.0f} },
				{ XMFLOAT3{-0.5f,  0.5f,  0.5f}, XMFLOAT3{0.0f, 0.0f, 1.0f}, XMFLOAT2{0.50f, 1.0f / 3.0f} },
				{ XMFLOAT3{-0.5f, -0.5f,  0.5f}, XMFLOAT3{0.0f, 0.0f, 1.0f}, XMFLOAT2{0.50f, 2.0f / 3.0f} },

				{ XMFLOAT3{-0.5f,  0.5f, -0.5f}, XMFLOAT3{0.0f, 1.0f, 0.0f}, XMFLOAT2{0.25f, 1.0f / 3.0f} },// Top
				{ XMFLOAT3{-0.5f,  0.5f,  0.5f}, XMFLOAT3{0.0f, 1.0f, 0.0f}, XMFLOAT2{0.25f, 0.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f,  0.5f,  0.5f}, XMFLOAT3{0.0f, 1.0f, 0.0f}, XMFLOAT2{0.50f, 0.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f,  0.5f, -0.5f}, XMFLOAT3{0.0f, 1.0f, 0.0f}, XMFLOAT2{0.50f, 1.0f / 3.0f} },

				{ XMFLOAT3{-0.5f, -0.5f,  0.5f}, XMFLOAT3{0.0f, -1.0f, 0.0f}, XMFLOAT2{0.25f, 3.0f / 3.0f} },// Bottom
				{ XMFLOAT3{-0.5f, -0.5f, -0.5f}, XMFLOAT3{0.0f, -1.0f, 0.0f}, XMFLOAT2{0.25f, 2.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f, -0.5f, -0.5f}, XMFLOAT3{0.0f, -1.0f, 0.0f}, XMFLOAT2{0.50f, 2.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f, -0.5f,  0.5f}, XMFLOAT3{0.0f, -1.0f, 0.0f}, XMFLOAT2{0.50f, 3.0f / 3.0f} },

				{ XMFLOAT3{-0.5f, -0.5f,  0.5f}, XMFLOAT3{-1.0f, 0.0f, 0.0f}, XMFLOAT2{0.00f, 2.0f / 3.0f} },// Left
				{ XMFLOAT3{-0.5f,  0.5f,  0.5f}, XMFLOAT3{-1.0f, 0.0f, 0.0f}, XMFLOAT2{0.00f, 1.0f / 3.0f} },
				{ XMFLOAT3{-0.5f,  0.5f, -0.5f}, XMFLOAT3{-1.0f, 0.0f, 0.0f}, XMFLOAT2{0.25f, 1.0f / 3.0f} },
				{ XMFLOAT3{-0.5f, -0.5f, -0.5f}, XMFLOAT3{-1.0f, 0.0f, 0.0f}, XMFLOAT2{0.25f, 2.0f / 3.0f} },

				{ XMFLOAT3{ 0.5f, -0.5f, -0.5f}, XMFLOAT3{1.0f, 0.0f, 0.0f}, XMFLOAT2{0.50f, 2.0f / 3.0f} },// Right
				{ XMFLOAT3{ 0.5f,  0.5f, -0.5f}, XMFLOAT3{1.0f, 0.0f, 0.0f}, XMFLOAT2{0.50f, 1.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f,  0.5f,  0.5f}, XMFLOAT3{1.0f, 0.0f, 0.0f}, XMFLOAT2{0.75f, 1.0f / 3.0f} },
				{ XMFLOAT3{ 0.5f, -0.5f,  0.5f}, XMFLOAT3{1.0f, 0.0f, 0.0f}, XMFLOAT2{0.75f, 2.0f / 3.0f} },
			};

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

			m_stride = sizeof(vertices[0]);
			m_indexCount = ARRAYSIZE(indices);

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

			std::filesystem::path filepath = "C:/dev/Sora/Sora/asset/texture/cubemap.bmp";
			hr = CreateWICTextureFromFile(device, filepath.wstring().c_str(), nullptr, m_texture.GetAddressOf()
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
			const auto context = m_graphics->GetContext();
			context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &m_offset);
			context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
			context->DrawIndexed(m_indexCount, 0, 0);
		}

	private:
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11ShaderResourceView> m_texture;
		UINT m_stride = 0;
		UINT m_offset = 0;
		UINT m_indexCount = 0;
		Graphics* m_graphics = nullptr;
	};
}
