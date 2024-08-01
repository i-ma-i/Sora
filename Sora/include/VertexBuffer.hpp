#pragma once

namespace sora
{
	template<class VertexType>
	class VertexBuffer
	{
	public:
		VertexBuffer(Graphics* graphics, const std::vector<VertexType>& vertices)
			: m_graphics(graphics)
			, m_stride(sizeof(VertexType))
			, m_offset(0)
		{
			const auto device = graphics->GetDevice();

			D3D11_BUFFER_DESC desc = {};
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.ByteWidth = static_cast<UINT>(m_stride * vertices.size());
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA vertexData = {};
			vertexData.pSysMem = vertices.data();

			HRESULT hr = device->CreateBuffer(&desc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create vertex buffer. HRESULT: {:#X}", hr);
				__debugbreak();
			}
		}

		void SetPipeline() const
		{
			m_graphics->GetContext()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &m_offset);
		}

	private:
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		UINT m_stride = 0;
		UINT m_offset = 0;
		Graphics* m_graphics = nullptr;
	};
}
