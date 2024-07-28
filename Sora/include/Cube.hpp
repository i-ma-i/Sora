#pragma once

namespace
{
	using namespace DirectX;

	//   5--------6
	//  /|       /|
	// 1--------2 |
	// | |      | |
	// | 4------|-7
	// |/       |/
	// 0--------3

	constexpr XMFLOAT3 normals[] =
	{
		{ 0.0f,  0.0f, -1.0f},// Front
		{ 0.0f,  0.0f,  1.0f},// Back
		{ 0.0f,  1.0f,  0.0f},// Top
		{ 0.0f, -1.0f,  0.0f},// Bottom
		{-1.0f,  0.0f,  0.0f},// Left
		{ 1.0f,  0.0f,  0.0f},// Right
	};

	// 頂点データを作成する。
	constexpr float half = 0.5f;
	const VertexPositionColor vertices[] =
	{
		{XMFLOAT3(-half, -half, -half),XMFLOAT4(Colors::Red)},
		{XMFLOAT3(-half,  half, -half),XMFLOAT4(Colors::Green)},
		{XMFLOAT3( half,  half, -half),XMFLOAT4(Colors::Blue)},
		{XMFLOAT3( half, -half, -half),XMFLOAT4(Colors::Yellow)},
		{XMFLOAT3(-half, -half,  half),XMFLOAT4(0.0f,1.0f,1.0f,1.0f)},
		{XMFLOAT3(-half,  half,  half),XMFLOAT4(Colors::Purple)},
		{XMFLOAT3( half,  half,  half),XMFLOAT4(Colors::Pink)},
		{XMFLOAT3( half, -half,  half),XMFLOAT4(Colors::Orange)},
	};

	// インデックスデータを作成する。
	constexpr UINT16 indices[] =
	{
		0, 1, 2, 0, 2, 3,// Front
		7, 6, 5, 7, 5, 4,// Back
		1, 5, 6, 1, 6, 2,// Top
		3, 7, 4, 3, 4, 0,// Bottom
		4, 5, 1, 4, 1, 0,// Left
		3, 2, 6, 3, 6, 7,//	Right
	};
}

namespace sora
{
	class Cube
	{
	public:
		Cube(Graphics* graphics)
			: m_stride(sizeof(vertices[0]))
			, m_offset(0)
			, m_indexCount(ARRAYSIZE(indices))
			, m_graphics(graphics)
		{
			using namespace DirectX;
			using namespace DirectX::SimpleMath;

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

			HRESULT hr = graphics->GetDevice()->CreateBuffer(&desc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf());
			if (FAILED(hr))
				LOG_ERROR("Failed to create vertex buffer. HRESULT: {:#X}", hr);

			// インデックスバッファを作成する。
			desc.ByteWidth = sizeof(indices);
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA indexData = {};
			indexData.pSysMem = indices;

			hr = graphics->GetDevice()->CreateBuffer(&desc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf());
			if (FAILED(hr))
				LOG_ERROR("Failed to create index buffer. HRESULT: {:#X}", hr);
		}
		~Cube()
		{
			m_graphics = nullptr;
		}

		void Draw() const
		{
			const auto context = m_graphics->GetContext();
			context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &m_offset);
			context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			context->DrawIndexed(m_indexCount, 0, 0);
		}

	private:
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		UINT m_stride = 0;
		UINT m_offset = 0;
		UINT m_indexCount = 0;
		Graphics* m_graphics = nullptr;
	};
}
