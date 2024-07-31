#include "Application.hpp"
#include "Config.hpp"
#include "Window.hpp"
#include "Engine.hpp"
#include "IKeyboard.hpp"
#include "IMouse.hpp"
#include "Graphics.hpp"
#include "AssetRegistry.hpp"
#include "GUI.hpp"
#include "Model.hpp"
#include "Quad.hpp"
#include "Cube.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "ConstantBuffer.hpp"

using namespace DirectX;
using path = std::filesystem::path;

namespace sora
{
	namespace
	{
		struct CBTransform
		{
			DirectX::SimpleMath::Matrix World;
			DirectX::SimpleMath::Matrix WVP;
		};
		
		bool s_initialized = false;
		bool s_running = false;
		std::unique_ptr<Window> s_window;
		std::unique_ptr<Engine> s_engine;
		std::unique_ptr<Graphics> s_graphics;
		std::unique_ptr<AssetRegistry> s_assetRegistry;
		std::unique_ptr<GUI> s_gui;
		std::unique_ptr<Camera> s_camera;
		std::unique_ptr<DirectionalLight> s_light;
		std::unique_ptr<ConstantBuffer<CBTransform>> s_cbTransform;
		std::unique_ptr<ConstantBuffer<CBLight>> s_cbLight;
		std::unique_ptr<Quad> s_quad;
		std::unique_ptr<Cube> s_cube;
		std::unique_ptr<Sphere> s_sphere;
		std::unique_ptr<Plane> s_plane;
		ComPtr<ID3D11ShaderResourceView> s_invalidTexture;

		bool Create()
		{
			Config::Create(std::filesystem::current_path() / "asset/config.yaml");
			Logger::Create();

			LOG_INFO("Creating...");

			// Engineを作成する。
			s_engine = std::make_unique<Engine>();

			// SDLを初期化する。
			if (SDL_Init(SDL_INIT_VIDEO) != 0)
			{
				LOG_ERROR("[SDL] Failed to initialize.  error: {}", SDL_GetError());
				return false;
			}

			s_window = std::make_unique<Window>();
			s_graphics = std::make_unique<Graphics>(s_window->GetHWND());
			s_assetRegistry = std::make_unique<AssetRegistry>(s_graphics.get());
			s_camera = std::make_unique<Camera>();
			s_light = std::make_unique<DirectionalLight>();

			// 定数バッファを作成する。
			s_cbTransform = std::make_unique<ConstantBuffer<CBTransform>>(s_graphics->GetDevice());
			s_cbTransform->SetPipeline(s_graphics->GetContext(), 0);
			s_cbLight = std::make_unique<ConstantBuffer<CBLight>>(s_graphics->GetDevice());
			s_cbLight->SetPipeline(s_graphics->GetContext(), 1);

			// プリミティブを作成する。
			s_quad = std::make_unique<Quad>(s_graphics.get());
			s_cube = std::make_unique<Cube>(s_graphics.get());
			s_sphere = std::make_unique<Sphere>(s_graphics.get());
			s_plane = std::make_unique<Plane>(s_graphics.get());

			// imguiを初期化する。
			s_gui = std::make_unique<GUI>(s_window.get(), s_graphics.get(), s_camera.get(), s_light.get());


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

			static SDL_Event event;
			static int mouseWheel = 0;
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

			// オブジェクトを描画する。
			{
				CBLight light;
				light.Direction = s_light->GetDirection();
				light.CameraPosition = DirectX::SimpleMath::Vector4(s_camera->GetPosition().x, s_camera->GetPosition().y, s_camera->GetPosition().z, 0.0f);
				s_cbLight->Update(s_graphics->GetContext(), light);

				s_graphics->GetContext()->PSSetShaderResources(0, 1, s_invalidTexture.GetAddressOf());

				CBTransform transform;
				const auto viewProjection = s_camera->GetViewProjection();

				transform.World = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-3.0f, 0.0f, 0.0f));
				transform.WVP = transform.World * viewProjection;
				transform.WVP = transform.WVP.Transpose();
				s_cbTransform->Update(s_graphics->GetContext(), transform);
				s_quad->Draw();
				transform.World = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(3.0f, 0.0f, 0.0f));
				transform.WVP = transform.World * viewProjection;
				transform.WVP = transform.WVP.Transpose();
				s_cbTransform->Update(s_graphics->GetContext(), transform);
				s_quad->Draw();

				transform.World = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-1.5f, 0.0f, 0.0f));
				transform.WVP = transform.World * viewProjection;
				transform.WVP = transform.WVP.Transpose();
				s_cbTransform->Update(s_graphics->GetContext(), transform);
				s_cube->Draw();
				transform.World = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(1.5f, 0.0f, 0.0f));
				transform.WVP = transform.World * viewProjection;
				transform.WVP = transform.WVP.Transpose();
				s_cbTransform->Update(s_graphics->GetContext(), transform);
				s_cube->Draw();

				transform.World = DirectX::SimpleMath::Matrix::Identity;
				transform.WVP = transform.World * viewProjection;
				transform.WVP = transform.WVP.Transpose();
				s_cbTransform->Update(s_graphics->GetContext(), transform);
				s_sphere->Draw();

				transform.World = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, -0.5f, 0.0f));
				transform.WVP = transform.World * viewProjection;
				transform.WVP = transform.WVP.Transpose();
				s_cbTransform->Update(s_graphics->GetContext(), transform);
				s_plane->Draw();
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
