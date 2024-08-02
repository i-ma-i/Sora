#pragma once

namespace sora
{
	class ConstantBuffer
	{
		using Matrix = DirectX::SimpleMath::Matrix;
		using Vector3 = DirectX::SimpleMath::Vector3;

	public:
		ConstantBuffer(Graphics* graphics)
			: m_graphics(graphics)
		{
			D3D11_BUFFER_DESC desc = {};
			desc.ByteWidth = sizeof(Coordinate);
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			const auto device = graphics->GetDevice();
			const auto context = graphics->GetContext();

			// 座標変換用のコンスタントバッファを作成する。
			HRESULT hr = device->CreateBuffer(&desc, nullptr, m_coordinate.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create constant buffer. HRESULT: {:#X}", hr);
				__debugbreak();
			}
			context->VSSetConstantBuffers(0, 1, m_coordinate.GetAddressOf());

			// カメラ用のコンスタントバッファを作成する。
			desc.ByteWidth = sizeof(Camera);
			hr = device->CreateBuffer(&desc, nullptr, m_camera.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create constant buffer. HRESULT: {:#X}", hr);
				__debugbreak();
			}
			context->PSSetConstantBuffers(1, 1, m_camera.GetAddressOf());

			// ディレクショナルライト用のコンスタントバッファを作成する。
			desc.ByteWidth = sizeof(DirectionalLight);
			hr = device->CreateBuffer(&desc, nullptr, m_directionalLight.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create constant buffer. HRESULT: {:#X}", hr);
				__debugbreak();
			}
			context->PSSetConstantBuffers(2, 1, m_directionalLight.GetAddressOf());
		}

		void UpdateCoordinate(const Matrix& world, const Matrix& view, const Matrix& projection)
		{
			const auto context = m_graphics->GetContext();

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HRESULT hr = context->Map(m_coordinate.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to map constant buffer.");
				__debugbreak();
			}

			Coordinate* data = reinterpret_cast<Coordinate*>(mappedResource.pData);
			data->World = world;
			data->WVP = world * view * projection;
			data->World.Transpose();
			data->WVP.Transpose();

			context->Unmap(m_coordinate.Get(), 0);
		}

		void UpdateCamera(const Vector3& position)
		{
			const auto context = m_graphics->GetContext();

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HRESULT hr = context->Map(m_camera.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to map constant buffer.");
				__debugbreak();
			}

			Camera* data = reinterpret_cast<Camera*>(mappedResource.pData);
			data->Position = position;
			data->Padding = 0.0f;

			context->Unmap(m_camera.Get(), 0);
		}

		void UpdateDirectionalLight(const Vector3& direction, const Vector3& color, const Vector3& ambient)
		{
			const auto context = m_graphics->GetContext();

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HRESULT hr = context->Map(m_directionalLight.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to map constant buffer.");
				__debugbreak();
			}

			DirectionalLight* data = reinterpret_cast<DirectionalLight*>(mappedResource.pData);
			data->Direction = direction;
			data->PaddingDirection = 0.0f;
			data->Color = color;
			data->PaddingColor = 0.0f;
			data->Ambient = ambient;
			data->PaddingAmbient = 0.0f;

			context->Unmap(m_directionalLight.Get(), 0);
		}

	private:
		struct Coordinate
		{
			Matrix World;
			Matrix WVP;
		};

		struct Camera
		{
			Vector3 Position;
			float Padding;
		};

		struct DirectionalLight
		{
			Vector3 Direction;
			float PaddingDirection;
			Vector3 Color;
			float PaddingColor;
			Vector3 Ambient;
			float PaddingAmbient;
		};

	private:
		ComPtr<ID3D11Buffer> m_coordinate;
		ComPtr<ID3D11Buffer> m_camera;
		ComPtr<ID3D11Buffer> m_directionalLight;
		Graphics* m_graphics = nullptr;
	};
}
