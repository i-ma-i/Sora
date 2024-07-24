#pragma once
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_dx11.h>

#include <SDL2/SDL.h>

#include "MouseInput.hpp"
#include "Camera.hpp"

namespace sora
{
	class GUI
	{
	public:
		GUI(SDL_Window* window, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			ImGui_ImplSDL2_InitForD3D(window);
			ImGui_ImplDX11_Init(device, deviceContext);
		}

		~GUI()
		{
			// Cleanup
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplSDL2_Shutdown();
			ImGui::DestroyContext();
		}

		void ProcessEvent(const SDL_Event* event) const
		{
			ImGui_ImplSDL2_ProcessEvent(event);
		}

		void Begin() const
		{
			// Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
		}
		void End() const
		{
			// Rendering
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		void MouseCapture() const
		{
			MouseInput mouse;
			ImGui::Begin("Mouse Capture");
			{
				ImGui::Text("--- Left Button ---");
				ImGui::Text("Clicked : %s", mouse.LeftClicked() ? "!" : "");
				ImGui::Text("Pressed : %s", mouse.LeftPressed() ? "!" : "");
				ImGui::Text("Released: %s", mouse.LeftReleased() ? "!" : "");
				ImGui::Separator();

				ImGui::Text("--- Right Button ---");
				ImGui::Text("Clicked : %s", mouse.RightClicked() ? "!" : "");
				ImGui::Text("Pressed : %s", mouse.RightPressed() ? "!" : "");
				ImGui::Text("Released: %s", mouse.RightReleased() ? "!" : "");
				ImGui::Separator();

				ImGui::Text("--- Middle Button ---");
				ImGui::Text("Clicked : %s", mouse.MiddleClicked() ? "!" : "");
				ImGui::Text("Pressed : %s", mouse.MiddlePressed() ? "!" : "");
				ImGui::Text("Released: %s", mouse.MiddleReleased() ? "!" : "");
				ImGui::Separator();

				ImGui::Text("--- Absolute Position ---");
				ImGui::Text("X: %d", mouse.PositionX());
				ImGui::Text("Y: %d", mouse.PositionY());
				ImGui::Separator();

				ImGui::Text("--- Delta Position ---");
				ImGui::Text("DeltaX: %d", mouse.DeltaX());
				ImGui::Text("DeltaY: %d", mouse.DeltaY());
				ImGui::Separator();

				ImGui::Text("--- Wheel Value ---");
				ImGui::Text("Value: %d", mouse.WheelValue());
			}
			ImGui::End();
		}

		void CameraSetting(Camera& camera) const
		{
			ImGui::Begin("Camera Setting");
			{
				ImGui::SliderFloat("Move Speed", &camera.m_moveSpeed, 1.0f, 50.0f);

				ImGui::SliderFloat("Rotate Speed", &camera.m_rotateSpeed, 0.1, 5.0f);

				ImGui::SliderFloat("Zoom Speed", &camera.m_zoomSpeed, 1.0f, 100.0f);
			}
			ImGui::End();
		}
	};
}
