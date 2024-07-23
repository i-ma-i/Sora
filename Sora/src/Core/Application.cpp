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

#include <magic_enum.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <imgui.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "d3d11.lib")

using namespace DirectX::SimpleMath;
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
		HWND hWnd;
		SDL_Window* s_window = nullptr;
		std::unique_ptr<Model> model;
		std::unique_ptr<Quad> plane;

		static int WIDTH;
		static int HEIGHT;

		// 定数バッファ
		ComPtr<ID3D11Buffer> gConstantBuffer;

		// 定数バッファの作成
		struct ConstantBuffer
		{
			Matrix mvp;
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

		Matrix modelMatrix = Matrix::CreateTranslation(0.0f, 1.0f, 0.0f);
		Matrix planeMatrix = Matrix::CreateTranslation(Vector3::Zero);
		Matrix viewMatrix = Matrix::CreateLookAt(Vector3(0.0f, 1.0f, 0.0f), Vector3::Zero, Vector3::UnitY);
		Matrix projectionMatrix;

		// シェーダーオブジェクト
		ComPtr<ID3D11VertexShader> gVertexShader;
		ComPtr<ID3D11PixelShader> gPixelShader;
		ComPtr<ID3D11InputLayout> gInputLayout;

		// シェーダーの初期化
		bool InitShaders()
		{
			// シェーダーデータを作成
			ComPtr<ID3DBlob> vsBlob = ShaderLoader::CompileHLSLFromFile(Env::GetString("vertexShader"), ShaderStage::Vertex, "VS");
			ComPtr<ID3DBlob> psBlob = ShaderLoader::CompileHLSLFromFile(Env::GetString("pixelShader"), ShaderStage::Pixel, "PS");

			// シェーダーオブジェクトを作成
			auto hr = s_graphics->GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, gVertexShader.GetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create vertex shader. HRESULT: {:#X}", hr);
				return false;
			}

			hr = s_graphics->GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, gPixelShader.GetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create pixel shader. HRESULT: {:#X}", hr);
				return false;
			}

			// インプットレイアウトを作成
			hr = s_graphics->GetDevice()->CreateInputLayout(VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), gInputLayout.GetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create input layout. HRESULT: {:#X}", hr);
				return false;
			}

			// シェーダーと入力レイアウトを設定
			s_graphics->GetDC()->IASetInputLayout(gInputLayout.Get());
			s_graphics->GetDC()->VSSetShader(gVertexShader.Get(), nullptr, 0);
			s_graphics->GetDC()->PSSetShader(gPixelShader.Get(), nullptr, 0);

			return true;
		};

		bool Create()
		{
			// Loggerを作成する。
			Logger::Setup({
				std::filesystem::current_path() / Env::GetString("logger.filename"),
				magic_enum::enum_cast<spdlog::level::level_enum>(Env::GetString("logger.level")).value(),
				Env::GetString("logger.filePattern"),
				Env::GetString("logger.consolePattern")
			});

			// エンジンを作成する。
			s_engine = std::make_unique<Engine>();

			// アプリケーションを作成する。
			LOG_INFO("Creating...");

			// ウィンドウサイズを環境変数から取得する。
			WIDTH = Env::GetInt("window.width");
			HEIGHT = Env::GetInt("window.height");

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
				WIDTH, HEIGHT,
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


			if (!InitShaders()) {
				LOG_ERROR("Failed to initialize shaders.");
				return false;
			}
			if (!CreateConstantBuffer()) {
				LOG_ERROR("Failed to create constant buffer.");
				return false;
			}


			// imguiを初期化する。
			s_gui = std::make_unique<GUI>(s_window, s_graphics->GetDevice(), s_graphics->GetDC());

			// プロジェクション行列を設定
			projectionMatrix = Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4, (float)WIDTH / HEIGHT, 1.0f, 1000.0f);

			// アプリケーションの作成が正常に完了。
			LOG_INFO("Creation completed successfully.");
			s_initialized = true;
			s_running = true;

			model = std::make_unique<Model>(s_graphics->GetDevice(), s_graphics->GetDC(), Env::GetString("modelpath"));
			plane = std::make_unique<Quad>(s_graphics->GetDevice());

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
			while (SDL_PollEvent(&event) != 0)
			{
				s_gui->ProcessEvent(&event);

				if (event.type == SDL_QUIT)
				{
					return false;
				}
			}

			Engine::GetModule<IKeyboard>().Update();
			Engine::GetModule<IMouse>().Update();

			static float modelRotationY = 0.0f;

			// カメラの位置を更新
			static float y = 5.0f;
			static float radius = 10.0f;
			static Vector3 focus = Vector3::Zero;
			float camX = sin(modelRotationY) * radius;
			float camZ = cos(modelRotationY) * radius;
			viewMatrix = Matrix::CreateLookAt(Vector3(camX, y, camZ), focus, Vector3::UnitY);

			// レンダリング開始
			static float clearColor[4] = { 0.529411793f, 0.807843208f, 0.921568692f, 1.f }; // 背景色を設定
			s_graphics->Begin(clearColor);

			// プリミティブトポロジの設定
			s_graphics->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			s_graphics->GetDC()->VSSetConstantBuffers(0, 1, gConstantBuffer.GetAddressOf());

			// オブジェクトの描画
			cb.mvp = modelMatrix * viewMatrix * projectionMatrix;
			cb.mvp = cb.mvp.Transpose();
			s_graphics->GetDC()->UpdateSubresource(gConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
			model->Draw(s_graphics->GetDC());
			cb.mvp = planeMatrix * viewMatrix * projectionMatrix;
			cb.mvp = cb.mvp.Transpose();
			s_graphics->GetDC()->UpdateSubresource(gConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
			plane->Draw(s_graphics->GetDC());

			s_gui->Begin();
			{
				static bool wireframe = false;
				if (ImGui::Checkbox("Wireframe", &wireframe))
				{
					wireframe ? s_graphics->SetWireframeMode() : s_graphics->SetSolidMode();
				}

				ImGui::ColorEdit4("Clear Color", clearColor);

				static float cameraScale = 1.0f;
				if (ImGui::InputFloat("Camera Scale", &cameraScale))
				{
					projectionMatrix = Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4, (float)WIDTH / HEIGHT, cameraScale, cameraScale * 1000.0f);
				}
				ImGui::SliderAngle("Model Rotation Y", &modelRotationY, 0.0f, 360.0f);
				ImGui::SliderFloat("Camera Position Y", &y, 0.0f, cameraScale * 20.0f);
				ImGui::SliderFloat("Camera Radius", &radius, 1.0, cameraScale * 20.0f);
				ImGui::SliderFloat3("Camera Focus", &focus.x, 0.0f, cameraScale * 20.0f);
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
