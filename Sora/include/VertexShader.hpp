#pragma once

namespace sora
{
	class VertexShader
	{
	public:
		VertexShader(
			const std::filesystem::path& path,
			const ComPtr<ID3D11Device>& device,
			const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
			UINT NumElements
		);
		
		void Bind(const ComPtr<ID3D11DeviceContext>& context) const;

	private:
		ComPtr<ID3D11VertexShader> m_vertexShader;
		ComPtr<ID3D11InputLayout> m_inputLayout;
	};
}
