#pragma once

namespace sora
{
	class Sphere
	{
	public:
		Sphere(Graphics* graphics, float radius, UINT slices, UINT stacks)
			: m_graphics(graphics)
		{
			using namespace DirectX;
			using namespace DirectX::SimpleMath;

			std::vector<VertexPositionNormalTexture> vertices;
			std::vector<UINT16> indices;

			// 球の頂点データを作成する。
			for (UINT i = 0; i <= stacks; i++)
			{
				float v = 1.0f - (float)i / stacks;
				float phi = v * XM_PI;

				for (UINT j = 0; j <= slices; j++)
				{
					float u = (float)j / slices;
					float theta = u * XM_2PI;

					float x = sinf(phi) * cosf(theta);
					float y = cosf(phi);
					float z = sinf(phi) * sinf(theta);

					XMFLOAT3 normal = XMFLOAT3(x, y, z);
					XMFLOAT2 texCoord = XMFLOAT2(u, v);

					vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(x, y, z) * radius, normal, texCoord));
				}
			}

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

			const auto device = m_graphics->GetDevice();

			// 頂点バッファを作成する。
			m_stride = sizeof(VertexPositionNormalTexture);
			D3D11_BUFFER_DESC vertexBufferDesc{};
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = static_cast<UINT>(vertices.size() * m_stride);
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			D3D11_SUBRESOURCE_DATA vertexBufferData{};
			vertexBufferData.pSysMem = vertices.data();
			device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_vertexBuffer.GetAddressOf());

			// インデックスバッファを作成する。
			m_indexCount = static_cast<UINT>(indices.size());
			D3D11_BUFFER_DESC indexBufferDesc{};
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(UINT16) * m_indexCount);
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			D3D11_SUBRESOURCE_DATA indexBufferData{};
			indexBufferData.pSysMem = indices.data();
			device->CreateBuffer(&indexBufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());

			std::filesystem::path filepath = "C:/dev/Sora/Sora/asset/texture/ground.png";
			auto hr = CreateWICTextureFromFile(device, filepath.wstring().c_str(), nullptr, m_texture.GetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to load texture.  path[{}]", filepath.string());
				assert(false && "Failed to load texture.");
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
