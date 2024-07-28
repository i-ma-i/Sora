#pragma once

namespace sora
{
	template<typename BufferType>
	class ConstantBuffer
	{
	public:
		ConstantBuffer(const ComPtr<ID3D11Device>& device)
		{
			// TODO: 更新方法を指定できるようにする。

			D3D11_BUFFER_DESC desc = {};
			desc.ByteWidth = sizeof(BufferType);
			desc.Usage = D3D11_USAGE_DEFAULT;// TODO: 不明
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = 0;// TODO: 不明
			desc.MiscFlags = 0;// TODO: 不明
			desc.StructureByteStride = 0;// TODO: 不明

			auto hr = device->CreateBuffer(&desc, nullptr, m_buffer.ReleaseAndGetAddressOf());
			if (FAILED(hr)) {
				LOG_ERROR("Failed to create constant buffer. HRESULT: {:#X}", hr);
				DebugBreak();
			}
		}

		void Update(const ComPtr<ID3D11DeviceContext>& context, const BufferType& data)
		{
			context->UpdateSubresource(m_buffer.Get(), 0, nullptr, &data, 0, 0);
		}

		void SetPipeline(const ComPtr<ID3D11DeviceContext>& context, UINT slot) const
		{
			context->VSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
			context->PSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
		}

	private:
		ComPtr<ID3D11Buffer> m_buffer;
	};
}
