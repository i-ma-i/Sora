#pragma once

namespace sora
{
	class IndexBuffer16
	{
	public:
		IndexBuffer16(Graphics* graphics, void* indices, size_t indexCount)
			: m_graphics(graphics)
			, m_indexCount(static_cast<UINT>(indexCount))
		{
			const auto device = graphics->GetDevice();

			D3D11_BUFFER_DESC desc = {};
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.ByteWidth = sizeof(UINT16) * m_indexCount;
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA indexData = {};
			indexData.pSysMem = indices;

			HRESULT hr = device->CreateBuffer(&desc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create index buffer. HRESULT: {:#X}", hr);
				__debugbreak();
			}
		}

		void SetPipeline() const
		{
			m_graphics->GetContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		}

		inline UINT GetIndexCount() const
		{
			return m_indexCount;
		}

	private:
		ComPtr<ID3D11Buffer> m_indexBuffer;
		UINT m_indexCount = 0;
		Graphics* m_graphics = nullptr;
	};
}
