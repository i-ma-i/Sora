#pragma once
#include "VertexShader.hpp"
#include "PixelShader.hpp"

namespace
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	enum Face
	{
		Front = 0,
		Back,
		Top,
		Bottom,
		Left,
		Right,
	};
	//   *--------*
	//  /|       /|
	// *--------* |
	// | |      | |
	// | *------|-*
	// |/       |/
	// *--------*
	
	// 頂点データを作成する。
	const VertexPositionNormalTexture vertices[] =
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

			const auto device = graphics->GetDevice();
			const auto context = graphics->GetContext();

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

			{// 法線を描画するためのリソースを作成する。
				constexpr XMFLOAT4 WHITE{ 1.0f,1.0f,1.0f,1.0f };
				const VertexPositionColor normalLines[] =
				{
					{vertices[0].position,  WHITE}, {vertices[0].position + vertices[0].normal,   WHITE},
					{vertices[1].position,  WHITE}, {vertices[1].position + vertices[1].normal,   WHITE},
					{vertices[2].position,  WHITE}, {vertices[2].position + vertices[2].normal,   WHITE},
					{vertices[3].position,  WHITE}, {vertices[3].position + vertices[3].normal,   WHITE},
					{vertices[4].position,  WHITE}, {vertices[4].position + vertices[4].normal,   WHITE},
					{vertices[5].position,  WHITE}, {vertices[5].position + vertices[5].normal,   WHITE},
					{vertices[6].position,  WHITE}, {vertices[6].position + vertices[6].normal,   WHITE},
					{vertices[7].position,  WHITE}, {vertices[7].position + vertices[7].normal,   WHITE},
					{vertices[8].position,  WHITE}, {vertices[8].position + vertices[8].normal,   WHITE},
					{vertices[9].position,  WHITE}, {vertices[9].position + vertices[9].normal,   WHITE},
					{vertices[10].position, WHITE}, {vertices[10].position + vertices[10].normal, WHITE},
					{vertices[11].position, WHITE}, {vertices[11].position + vertices[11].normal, WHITE},
					{vertices[12].position, WHITE}, {vertices[12].position + vertices[12].normal, WHITE},
					{vertices[13].position, WHITE}, {vertices[13].position + vertices[13].normal, WHITE},
					{vertices[14].position, WHITE}, {vertices[14].position + vertices[14].normal, WHITE},
					{vertices[15].position, WHITE}, {vertices[15].position + vertices[15].normal, WHITE},
					{vertices[16].position, WHITE}, {vertices[16].position + vertices[16].normal, WHITE},
					{vertices[17].position, WHITE}, {vertices[17].position + vertices[17].normal, WHITE},
					{vertices[18].position, WHITE}, {vertices[18].position + vertices[18].normal, WHITE},
					{vertices[19].position, WHITE}, {vertices[19].position + vertices[19].normal, WHITE},
					{vertices[20].position, WHITE}, {vertices[20].position + vertices[20].normal, WHITE},
					{vertices[21].position, WHITE}, {vertices[21].position + vertices[21].normal, WHITE},
					{vertices[22].position, WHITE}, {vertices[22].position + vertices[22].normal, WHITE},
					{vertices[23].position, WHITE}, {vertices[23].position + vertices[23].normal, WHITE},
				};

				desc.ByteWidth = sizeof(normalLines);
				D3D11_SUBRESOURCE_DATA normalData = {};
				normalData.pSysMem = normalLines;
				hr = device->CreateBuffer(&desc, &normalData, m_normalVertexBuffer.ReleaseAndGetAddressOf());
				if (FAILED(hr))
					LOG_ERROR("Failed to create normal vertex buffer. HRESULT: {:#X}", hr);

				m_normalVertexShader = std::make_unique<VertexShader>(std::filesystem::current_path() / "asset/shader/ColorVS.cso", device, VertexPositionColor::InputElements, VertexPositionColor::InputElementCount);
				m_normalPixelShader = std::make_unique<PixelShader>(device, std::filesystem::current_path() / "asset/shader/ColorPS.cso");
			}

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
		~Cube()
		{
			m_graphics = nullptr;
		}

		void Draw() const
		{
			const auto context = m_graphics->GetContext();
			context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &m_offset);
			context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
			context->DrawIndexed(m_indexCount, 0, 0);

			{// 法線を描画する。
				UINT stride = sizeof(VertexPositionColor);
				context->IASetVertexBuffers(0, 1, m_normalVertexBuffer.GetAddressOf(), &stride, &m_offset);
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
				m_normalVertexShader->Bind(context);
				m_normalPixelShader->Bind(context);
				for (int i = 0; i < 8; i++)
				{
					context->Draw(2, i * 2);
				}
			}
		}

	private:
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11ShaderResourceView> m_texture;
		UINT m_stride = 0;
		UINT m_offset = 0;
		UINT m_indexCount = 0;
		Graphics* m_graphics = nullptr;


		ComPtr<ID3D11Buffer> m_normalVertexBuffer;
		std::unique_ptr<VertexShader> m_normalVertexShader;
		std::unique_ptr<PixelShader> m_normalPixelShader;
	};
}
