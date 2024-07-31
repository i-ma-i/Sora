#pragma once
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_dx11.h>

#include <SDL2/SDL.h>

#include "Engine.hpp"
#include "Window.hpp"
#include "MouseInput.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"
#include "DirectionalLight.hpp"

namespace sora
{
#if defined(DEBUG) || defined(_DEBUG)
	class GUI
	{
	private:
		Window* m_window;
		Graphics* m_graphics;
		Camera* m_camera;
		DirectionalLight* m_light;

	private:
		bool m_openMouseCapture = false;
		bool m_openGraphicsConfig = false;
		bool m_openCameraConfig = false;
		bool m_openLightConfig = false;

	public:
		GUI(Window* window, Graphics* graphics, Camera* camera, DirectionalLight* light)
			: m_window(window)
			, m_graphics(graphics)
			, m_camera(camera)
			, m_light(light)
			, m_openMouseCapture(Config::GetBool("GUI.m_openMouseCapture"))
			, m_openGraphicsConfig(Config::GetBool("GUI.m_openGraphicsConfig"))
			, m_openCameraConfig(Config::GetBool("GUI.m_openCameraConfig"))
			, m_openLightConfig(Config::GetBool("GUI.m_openLightConfig"))
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			ImGui_ImplSDL2_InitForD3D(m_window->m_window);
			ImGui_ImplDX11_Init(m_graphics->GetDevice(), m_graphics->GetContext());
		}
		~GUI()
		{
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplSDL2_Shutdown();
			ImGui::DestroyContext();
		}

		void ProcessEvent(const SDL_Event* event) const
		{
			ImGui_ImplSDL2_ProcessEvent(event);
		}

		void Draw()
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
			{
				MenuBar();
				MouseCapture();
				GraphicsConfig();
				CameraConfig();
				LightConfig();
			}
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

	private:
		void MenuBar()
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Config"))
				{
					if (ImGui::MenuItem("Mouse Capture", nullptr, m_openMouseCapture)) m_openMouseCapture = !m_openMouseCapture;
					if (ImGui::MenuItem("Graphics Config", nullptr, m_openGraphicsConfig)) m_openGraphicsConfig = !m_openGraphicsConfig;
					if (ImGui::MenuItem("Camera Config", nullptr, m_openCameraConfig)) m_openCameraConfig = !m_openCameraConfig;
					if (ImGui::MenuItem("Light Config", nullptr, m_openLightConfig)) m_openLightConfig = !m_openLightConfig;

					ImGui::EndMenu();
				}
				
				ImGui::EndMainMenuBar();
			}
		}

		void MouseCapture() const
		{
			if (!m_openMouseCapture) return;

			MouseInput mouse;
			ImGui::Begin("Mouse Capture");
			{
				ImGui::SeparatorText("Left Button");
				ImGui::Text("Clicked : %s", mouse.LeftClicked() ? "!" : "");
				ImGui::Text("Pressed : %s", mouse.LeftPressed() ? "!" : "");
				ImGui::Text("Released: %s", mouse.LeftReleased() ? "!" : "");

				ImGui::SeparatorText("Right Button");
				ImGui::Text("Clicked : %s", mouse.RightClicked() ? "!" : "");
				ImGui::Text("Pressed : %s", mouse.RightPressed() ? "!" : "");
				ImGui::Text("Released: %s", mouse.RightReleased() ? "!" : "");

				ImGui::SeparatorText("Middle Button");
				ImGui::Text("Clicked : %s", mouse.MiddleClicked() ? "!" : "");
				ImGui::Text("Pressed : %s", mouse.MiddlePressed() ? "!" : "");
				ImGui::Text("Released: %s", mouse.MiddleReleased() ? "!" : "");

				ImGui::SeparatorText("Absolute Position");
				ImGui::Text("X: %d", mouse.PositionX());
				ImGui::Text("Y: %d", mouse.PositionY());

				ImGui::SeparatorText("Delta Position");
				ImGui::Text("DeltaX: %d", mouse.DeltaX());
				ImGui::Text("DeltaY: %d", mouse.DeltaY());

				ImGui::SeparatorText("Wheel Value");
				ImGui::Text("Value: %d", mouse.WheelValue());
			}
			ImGui::End();
		}

		void GraphicsConfig() const
		{
			if (!m_openGraphicsConfig) return;

			ImGui::Begin("Graphics Config");
			{
				static bool wireframe = false;
				ImGui::Checkbox("Wireframe", &wireframe);
				wireframe ? m_graphics->SetWireframeMode() : m_graphics->SetSolidMode();
			}
			ImGui::End();
		}

		void CameraConfig() const
		{
			if (!m_openCameraConfig) return;

			ImGui::Begin("Camera Setting");
			{
				ImGui::SeparatorText("Attitude");
				ImGui::InputFloat3("Position", &m_camera->m_position.x);
				ImGui::Text("Forward: (%.2f, %.2f, %.2f)", m_camera->GetForwardLH().x, m_camera->GetForwardLH().y, m_camera->GetForwardLH().z);
				ImGui::SliderAngle("Yaw", &m_camera->m_yawRad, 0.0f, 360.0f);
				ImGui::SliderAngle("Pitch", &m_camera->m_pitchRad, -90.0f, 90.0f);

				ImGui::SeparatorText("Speed");
				static float minSpeed = 0.01f, maxSpeed = 2.0f;
				ImGui::SliderFloat("Camera Speed", &m_camera->m_moveSpeed, minSpeed, maxSpeed);
				ImGui::InputFloat("Min", &minSpeed);
				ImGui::InputFloat("Max", &maxSpeed);
			}
			ImGui::End();
		}

		void LightConfig() const
		{
			if (!m_openLightConfig) return;

			ImGui::Begin("Light Setting");
			{
				ImGui::SeparatorText("Directional Light");
				ImGui::SliderFloat3("Direction", &m_light->m_direction.x, -1.0f, 1.0f);
			}
			ImGui::End();
		}
	};
#else
	class GUI
	{
	public:
		GUI(Window* window, Graphics* graphics, Camera* camera) {}
		~GUI() {}

		void ProcessEvent(const SDL_Event* event) const {}

		void Draw() {}
	};
#endif
}
