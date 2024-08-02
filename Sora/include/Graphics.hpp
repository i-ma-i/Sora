#pragma once

namespace sora
{
	class Graphics
	{
	public:
		Graphics(HWND hWnd);

		void Begin() const;
		void End() const;

		ID3D11Device* GetDevice() const { return m_device.Get(); }
		ID3D11DeviceContext* GetContext() const { return m_immidiateContext.Get(); }

		void SetWireframeMode() const
		{
			m_immidiateContext->RSSetState(m_rsWireframe.Get());
		}
		void SetSolidMode() const
		{
			m_immidiateContext->RSSetState(m_rsSolid.Get());
		}

	private:
		bool m_enableWireframe = false;
		D3D11_CULL_MODE m_cullMode = D3D11_CULL_NONE;
		bool m_frontCounterClockwise = false;

	private:
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_immidiateContext;
		ComPtr<IDXGISwapChain> m_swapchain;
		ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		ComPtr<ID3D11RasterizerState> m_rsSolid, m_rsWireframe;
		ComPtr<ID3D11SamplerState> m_samplerState;

	private:
		friend class GUI;
	};
}
