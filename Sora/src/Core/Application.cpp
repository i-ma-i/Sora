#define TINYOBJLOADER_IMPLEMENTATION
#include "Application.hpp"
#include "Config.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include "Engine.hpp"
#include "IKeyboard.hpp"
#include "IMouse.hpp"
#include "Graphics.hpp"
#include "GUI.hpp"
#include "ShaderLoader.hpp"
#include "Model.hpp"
#include "Primitive.hpp"
#include "Camera.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <imgui.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

namespace sora
{
	namespace
	{
		bool s_initialized = false;
		bool s_running = false;
		std::unique_ptr<Window> s_window;
		std::unique_ptr<Engine> s_engine;
		std::unique_ptr<Graphics> s_graphics;
		std::unique_ptr<GUI> s_gui;
		std::unique_ptr<Camera> s_camera;
		std::unique_ptr<VertexShader> s_vertexShader;
		std::unique_ptr<PixelShader> s_pixelShader;
		std::unique_ptr<Quad> s_plane;
		ComPtr<ID3D11ShaderResourceView> s_invalidTexture;

		// 定数バッファ
		ComPtr<ID3D11Buffer> cbTransform;

		// 定数バッファの作成
		struct ConstantBufferTransform
		{
			DirectX::SimpleMath::Matrix World;
			DirectX::SimpleMath::Matrix WVP;
		};
		ConstantBufferTransform s_transform;

		bool CreateConstantBuffer()
		{
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(ConstantBufferTransform);
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			HRESULT hr = s_graphics->GetDevice()->CreateBuffer(&bd, nullptr, cbTransform.GetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create constant buffer. HRESULT: {:#X}", hr);
				DebugBreak();
			}

			return true;
		}

		bool Create()
		{
			Config::Create(std::filesystem::current_path() / "asset/config.yaml");
			Logger::Create();

			LOG_INFO("Creating...");

			s_engine = std::make_unique<Engine>();

			// SDLを初期化する。
			if (SDL_Init(SDL_INIT_VIDEO) != 0)
			{
				LOG_ERROR("[SDL] Failed to initialize.  error: {}", SDL_GetError());
				return false;
			}

			s_window = std::make_unique<Window>();
			s_graphics = std::make_unique<Graphics>(s_window->GetHWND());
			s_camera = std::make_unique<Camera>();

			// シェーダーを作成する。
			s_vertexShader = std::make_unique<VertexShader>(
				std::filesystem::current_path() / Config::GetString("shader.basicVS"),
				s_graphics->GetDevice(),
				DirectX::VertexPositionNormalTexture::InputElements,
				DirectX::VertexPositionNormalTexture::InputElementCount
			);
			s_pixelShader = std::make_unique<PixelShader>(s_graphics->GetDevice(), std::filesystem::current_path() / Config::GetString("shader.basicPS"));
			s_vertexShader->Bind(s_graphics->GetDC());
			s_pixelShader->Bind(s_graphics->GetDC());

			if (!CreateConstantBuffer()) {
				LOG_ERROR("Failed to create constant buffer.");
				return false;
			}

			s_plane = std::make_unique<Quad>(s_graphics->GetDevice());


			// imguiを初期化する。
			s_gui = std::make_unique<GUI>(s_window.get(), s_graphics.get(), s_camera.get());


			// 無効なテクスチャを作成する。
			{
				D3D11_TEXTURE2D_DESC textureDesc = {};
				textureDesc.Width = 1;
				textureDesc.Height = 1;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				textureDesc.SampleDesc.Count = 1;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

				D3D11_SUBRESOURCE_DATA initData = {};
				UINT32 pixelColor = 0xFFFC03F8; // ピンク色
				initData.pSysMem = &pixelColor;
				initData.SysMemPitch = sizeof(UINT32);

				ComPtr<ID3D11Texture2D> texture;
				if (FAILED(s_graphics->GetDevice()->CreateTexture2D(&textureDesc, &initData, texture.ReleaseAndGetAddressOf())))
				{
					__debugbreak();
				}

				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
				srvDesc.Format = textureDesc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = 1;

				// シェーダーリソースビューの作成
				if (FAILED(s_graphics->GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, s_invalidTexture.ReleaseAndGetAddressOf())))
				{
					__debugbreak();
				}
			}

			// アプリケーションの作成が正常に完了。
			LOG_INFO("Creation completed successfully.");
			s_initialized = true;
			s_running = true;

			return true;
		}

		void Destroy()
		{
			LOG_INFO("Destroy.");

			// SDLを終了する。
			s_window.reset();
			SDL_Quit();

			// Loggerを終了する。
			Logger::Cleanup();
		}
	}

	namespace Application
	{
		bool Update()
		{
			if (s_initialized == false)
			{
				Create();
			}

			if (s_running == false)
			{
				Destroy();
				return false;
			}

			SDL_Event event;
			int mouseWheel = 0;
			while (SDL_PollEvent(&event) != 0)
			{
				s_gui->ProcessEvent(&event);

				if (event.type == SDL_QUIT)
				{
					return false;
				}
				if(event.type == SDL_MOUSEWHEEL)
				{
					mouseWheel = event.wheel.y;
				}
			}

			Engine::GetModule<IKeyboard>()->Update();
			Engine::GetModule<IMouse>()->Update(mouseWheel);
			s_camera->Update(0.0167f);

			// 描画を開始する。
			s_graphics->Begin();

			// プリミティブトポロジを設定する。
			s_graphics->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			s_graphics->GetDC()->VSSetConstantBuffers(0, 1, cbTransform.GetAddressOf());

			// オブジェクトを描画する。
			{
				s_transform.WVP = DirectX::SimpleMath::Matrix::Identity * s_camera->GetViewProjection();
				s_graphics->GetDC()->UpdateSubresource(cbTransform.Get(), 0, nullptr, &s_transform, 0, 0);
				s_graphics->GetDC()->PSSetShaderResources(0, 1, s_invalidTexture.GetAddressOf());
				s_plane->Draw(s_graphics->GetDC());
			}

			// GUIを描画する。
			s_gui->Draw();

			// 描画を終了終了する。
			s_graphics->End();

			return true;
		}

		void Exit()
		{
			s_running = false;
		}
	}
}
