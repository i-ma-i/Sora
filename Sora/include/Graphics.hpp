#pragma once

namespace sora
{
	class Graphics
	{
	public:
		Graphics(HWND hWnd)
		{
			// ウィンドウのサイズを取得する。
			RECT rect;
			::GetClientRect(hWnd, &rect);
			const UINT width = rect.right - rect.left;
			const UINT height = rect.bottom - rect.top;

			// デバイスとスワップチェインを作成する。
			{
				DXGI_SWAP_CHAIN_DESC scDesc = {};
				scDesc.BufferCount = 2;
				scDesc.BufferDesc.Width = width;
				scDesc.BufferDesc.Height = height;
				scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				scDesc.BufferDesc.RefreshRate.Numerator = 60;
				scDesc.BufferDesc.RefreshRate.Denominator = 1;
				scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				scDesc.OutputWindow = hWnd;
				scDesc.SampleDesc.Count = 1;
				scDesc.SampleDesc.Quality = 0;
				scDesc.Windowed = TRUE;
				scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

				UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if DEBUG || _DEBUG
				deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

				D3D_FEATURE_LEVEL featureLevel;
				const D3D_FEATURE_LEVEL FEATURE_LEVELS[] = { D3D_FEATURE_LEVEL_11_1 };
				HRESULT hr = D3D11CreateDeviceAndSwapChain(
					nullptr,
					D3D_DRIVER_TYPE_HARDWARE,
					nullptr,
					deviceFlags,
					FEATURE_LEVELS,
					ARRAYSIZE(FEATURE_LEVELS),
					D3D11_SDK_VERSION,
					&scDesc,
					m_swapchain.ReleaseAndGetAddressOf(),
					m_device.ReleaseAndGetAddressOf(),
					&featureLevel,
					m_immidiateContext.ReleaseAndGetAddressOf()
				);
				if (FAILED(hr))
				{
					LOG_ERROR("Failed to D3D11CreateDeviceAndSwapChain. HRESULT: {:#X}", hr);
					__debugbreak();
				}
			}

			// レンダーターゲットを作成する。
			{
				ComPtr<ID3D11Texture2D> backBuffer;
				HRESULT hr = m_swapchain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
				if (FAILED(hr))
				{
					LOG_ERROR("Failed to get BackBuffer. HRESULT: {:#X}", hr);
					__debugbreak();
				}

				hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					LOG_ERROR("Failed to CreateRenderTargetView. HRESULT: {:#X}", hr);
					__debugbreak();
				}
			}

			// デプスステンシルビューを作成する。
			{
				D3D11_TEXTURE2D_DESC dsDesc = {};
				dsDesc.Width = width;
				dsDesc.Height = height;
				dsDesc.MipLevels = 1;
				dsDesc.ArraySize = 1;
				dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				dsDesc.SampleDesc.Count = 1;
				dsDesc.SampleDesc.Quality = 0;
				dsDesc.Usage = D3D11_USAGE_DEFAULT;
				dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				dsDesc.CPUAccessFlags = 0;
				dsDesc.MiscFlags = 0;

				ComPtr<ID3D11Texture2D> dsTexture;
				HRESULT hr = m_device->CreateTexture2D(&dsDesc, nullptr, dsTexture.GetAddressOf());
				if (FAILED(hr))
				{
					LOG_ERROR("Failed to create depth stencil texture. HRESULT: {:#X}", hr);
					__debugbreak();
				}

				D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc = {};
				dsViewDesc.Format = dsDesc.Format;
				dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				dsViewDesc.Texture2D.MipSlice = 0;

				hr = m_device->CreateDepthStencilView(dsTexture.Get(), &dsViewDesc, m_depthStencilView.ReleaseAndGetAddressOf());
				if (FAILED(hr)) {
					LOG_ERROR("Failed to create depth stencil view. HRESULT: {:#X}", hr);
					__debugbreak();
				}
			}

			// ラスタライザステートを作成する。
			{
				D3D11_RASTERIZER_DESC rasterizerDesc = {};
				rasterizerDesc.FillMode = D3D11_FILL_SOLID;
				rasterizerDesc.CullMode = D3D11_CULL_NONE; // バックフェイスカリングを有効にする
				rasterizerDesc.FrontCounterClockwise = false; // 時計回りがフロントフェイス(左手座標系)
				rasterizerDesc.DepthBias = 0;
				rasterizerDesc.SlopeScaledDepthBias = 0.0f;
				rasterizerDesc.DepthBiasClamp = 0.0f;
				rasterizerDesc.DepthClipEnable = true;
				rasterizerDesc.ScissorEnable = false;
				rasterizerDesc.MultisampleEnable = false;
				rasterizerDesc.AntialiasedLineEnable = false;

				HRESULT hr = m_device->CreateRasterizerState(&rasterizerDesc, m_rsSolid.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					LOG_ERROR("Failed to create rasterizer state. HRESULT: {:#X}", hr);
					__debugbreak();
				}

				rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
				hr = m_device->CreateRasterizerState(&rasterizerDesc, m_rsWireframe.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					LOG_ERROR("Failed to create rasterizer state. HRESULT: {:#X}", hr);
					__debugbreak();
				}

				SetSolidMode();
			}

			// サンプラーステートを作成する。
			{
				D3D11_SAMPLER_DESC samplerDesc = {};
				samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				samplerDesc.MinLOD = 0;
				samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

				HRESULT hr = m_device->CreateSamplerState(&samplerDesc, m_samplerState.ReleaseAndGetAddressOf());
				if (FAILED(hr)) {
					LOG_ERROR("Failed to create sampler state. HRESULT: {:#X}", hr);
					__debugbreak();
				}

				m_immidiateContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

				// TODO: ミップマップフィルタリング用のサンプラーステートを作成する
				// TODO: リニアフィルタリング用のサンプラーステートを作成する
				// TODO: ポイントフィルタリング用のサンプラーステートを作成する
				// TODO: サンプラーステートを切り替えるメソッドを作成する。
			}

			// ビューポートを設定する。
			{
				D3D11_VIEWPORT vp = {};
				vp.Width = static_cast<float>(width);
				vp.Height = static_cast<float>(height);
				vp.MinDepth = 0.0f;
				vp.MaxDepth = 1.0f;
				vp.TopLeftX = 0;
				vp.TopLeftY = 0;
				m_immidiateContext->RSSetViewports(1, &vp);
			}

			// TODO: 途中で個別に設定せずに、最後にパイプラインの設定をする。
		}

		void Begin(const float* clearColor) const
		{
			// レンダーターゲットとデプスステンシルビューを設定する。
			m_immidiateContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

			m_immidiateContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
			m_immidiateContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		void End() const
		{
			m_swapchain->Present(1, 0);
		}

		ID3D11Device* GetDevice() const { return m_device.Get(); }
		ID3D11DeviceContext* GetDC() const { return m_immidiateContext.Get(); }

		void SetWireframeMode() const
		{
			m_immidiateContext->RSSetState(m_rsWireframe.Get());
		}
		void SetSolidMode() const
		{
			m_immidiateContext->RSSetState(m_rsSolid.Get());
		}

	private:
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_immidiateContext;
		ComPtr<IDXGISwapChain> m_swapchain;
		ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		ComPtr<ID3D11RasterizerState> m_rsSolid, m_rsWireframe;
		ComPtr<ID3D11SamplerState> m_samplerState;
	};
}
