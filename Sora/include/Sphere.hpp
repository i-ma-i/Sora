﻿#pragma once
#include "AssetRegistry.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"

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

					XMFLOAT3 normal = { x, y, z };
					XMFLOAT2 texCoord = { u, v };

					vertices.emplace_back(XMFLOAT3(x, y, z) * 0.5f, normal, texCoord);
				}
			}
			m_stride = sizeof(VertexPositionNormalTexture);

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
			m_indexCount = static_cast<UINT>(indices.size());

			const auto device = m_graphics->GetDevice();

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

			std::filesystem::path filepath = "C:/dev/Sora/Sora/asset/texture/ground.png";
			hr = CreateWICTextureFromFile(device, filepath.wstring().c_str(), nullptr, m_texture.GetAddressOf());
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

			AssetRegistry::GetAsset<VertexShader>(VS)->SetPipeline();
			AssetRegistry::GetAsset<PixelShader>(PS)->SetPipeline();
			context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
			context->DrawIndexed(m_indexCount, 0, 0);
		}

	private:
		static constexpr std::string_view VS = "Shader.Basic.VS";
		static constexpr std::string_view PS = "Shader.Basic.PS";
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11ShaderResourceView> m_texture;
		UINT m_stride = 0;
		UINT m_offset = 0;
		UINT m_indexCount = 0;
		Graphics* m_graphics = nullptr;
	};
}
