#define TINYOBJLOADER_IMPLEMENTATION
#include "Application.hpp"
#include "Engine.hpp"
#include "Env.hpp"
#include "Logger.hpp"
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

#include <magic_enum.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <imgui.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "d3d11.lib")

using namespace DirectX;

namespace sora
{
	namespace
	{
		bool s_initialized = false;
		bool s_running = false;
		std::unique_ptr<Engine> s_engine;
		std::unique_ptr<Graphics> s_graphics;
		std::unique_ptr<GUI> s_gui;
		std::unique_ptr<Camera> s_camera;
		HWND hWnd;
		SDL_Window* s_window = nullptr;
		std::unique_ptr<VertexShader> s_vertexShader;
		std::unique_ptr<PixelShader> s_pixelShader;
		std::unique_ptr<Model> model;
		std::unique_ptr<Quad> plane;
		ComPtr<ID3D11ShaderResourceView> s_invalidTexture;

		// 定数バッファ
		ComPtr<ID3D11Buffer> gConstantBuffer;

		// 定数バッファの作成
		struct ConstantBuffer
		{
			DirectX::SimpleMath::Matrix mvp;
		};

		ConstantBuffer cb;
		bool CreateConstantBuffer()
		{
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(ConstantBuffer);
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			HRESULT hr = s_graphics->GetDevice()->CreateBuffer(&bd, nullptr, gConstantBuffer.GetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create constant buffer. HRESULT: {:#X}", hr);
				return false;
			}

			return true;
		}

		bool Create()
		{
			// Loggerを作成する。
			Logger::Setup({
				std::filesystem::current_path() / Env::GetString("logger.filename"),
				magic_enum::enum_cast<spdlog::level::level_enum>(Env::GetString("logger.level")).value(),
				Env::GetString("logger.filePattern"),
				Env::GetString("logger.consolePattern")
			});

			// アプリケーションを作成する。
			LOG_INFO("Creating...");

			// エンジンを作成する。
			s_engine = std::make_unique<Engine>();

			// SDLを初期化する。
			if (SDL_Init(SDL_INIT_VIDEO) != 0)
			{
				LOG_ERROR("[SDL] Failed to initialize.  error: {}", SDL_GetError());
				return false;
			}

			// ウィンドウを作成する。
			s_window = SDL_CreateWindow(
				Env::GetString("window.title").c_str(),
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				Env::GetInt("window.width"), Env::GetInt("window.height"),
				SDL_WINDOW_ALLOW_HIGHDPI
			);
			if (s_window == nullptr)
			{
				LOG_ERROR("[SDL] SDL_CreateWindow Error.  error: {}", SDL_GetError());
				return false;
			}

			// SDLウィンドウからハンドルを取得
			SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version);
			SDL_GetWindowWMInfo(s_window, &wmInfo);
			hWnd = (HWND)wmInfo.info.win.window;

			s_graphics = std::make_unique<Graphics>(hWnd);

			// カメラを作成する。
			Camera::Config config;
			config.position = {
				Env::GetFloat("camera.position[0]"),
				Env::GetFloat("camera.position[1]"),
				Env::GetFloat("camera.position[2]")
			};
			config.yawRad = Env::GetFloat("camera.yaw");
			config.pitchRad = Env::GetFloat("camera.pitch");
			config.moveSpeed = Env::GetFloat("camera.moveSpeed");
			config.rotateSpeedRad = Env::GetFloat("camera.rotateSpeed");
			config.zoomSpeed = Env::GetFloat("camera.zoomSpeed");
			config.nearZ = Env::GetFloat("camera.nearZ");
			config.farZ = Env::GetFloat("camera.farZ");
			s_camera = std::make_unique<Camera>(config);

			// シェーダーを作成する。
			s_vertexShader = std::make_unique<VertexShader>(s_graphics->GetDevice(), std::filesystem::current_path() / Env::GetString("shader.basicVS"));
			s_vertexShader->Bind(s_graphics->GetDC());
			s_pixelShader = std::make_unique<PixelShader>(s_graphics->GetDevice(), std::filesystem::current_path() / Env::GetString("shader.basicPS"));
			s_pixelShader->Bind(s_graphics->GetDC());

			if (!CreateConstantBuffer()) {
				LOG_ERROR("Failed to create constant buffer.");
				return false;
			}


			// imguiを初期化する。
			s_gui = std::make_unique<GUI>(s_window, s_graphics->GetDevice(), s_graphics->GetDC());

			// アプリケーションの作成が正常に完了。
			LOG_INFO("Creation completed successfully.");
			s_initialized = true;
			s_running = true;

			model = std::make_unique<Model>(s_graphics->GetDevice(), s_graphics->GetDC(), Env::GetString("modelpath"));
			plane = std::make_unique<Quad>(s_graphics->GetDevice());

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

			return true;
		}

		void Destroy()
		{
			LOG_INFO("Destroy.");

			// SDLを終了する。
			SDL_DestroyWindow(s_window);
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

			Engine::GetModule<IKeyboard>().Update();
			Engine::GetModule<IMouse>().Update(mouseWheel);
			s_camera->Update(0.0167f);

			// レンダリング開始
			static auto clearColor = DirectX::Colors::SkyBlue;
			s_graphics->Begin(clearColor);

			// プリミティブトポロジの設定
			s_graphics->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			s_graphics->GetDC()->VSSetConstantBuffers(0, 1, gConstantBuffer.GetAddressOf());

			// カメラ行列を取得する。
			const auto viewProjection = s_camera->GetViewProjection();

			// オブジェクトの描画
			{
				cb.mvp = viewProjection;
				s_graphics->GetDC()->UpdateSubresource(gConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
				model->Draw(s_graphics->GetDC());
				s_graphics->GetDC()->PSSetShaderResources(0, 1, s_invalidTexture.GetAddressOf());
				s_graphics->GetDC()->UpdateSubresource(gConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
				plane->Draw(s_graphics->GetDC());
			}

			s_gui->Begin();
			{
				ImGui::Begin("Settings");
				{
					static bool wireframe = false;
					
					if (ImGui::Checkbox("Wireframe", &wireframe))
					{
						static DirectX::XMVECTORF32 beforeColor;
						if (wireframe)
						{
							beforeColor = clearColor;
							clearColor = DirectX::Colors::Black;
							s_graphics->SetWireframeMode();
						}
						else
						{
							clearColor = beforeColor;
							s_graphics->SetSolidMode();
						}
					}
					if(!wireframe)
						ImGui::ColorEdit4("Clear Color", clearColor.f);
				}
				ImGui::End();
			}
			s_gui->End();

			// レンダリング終了
			s_graphics->End();

			return true;
		}

		void Exit()
		{
			s_running = false;
		}
	}
}
