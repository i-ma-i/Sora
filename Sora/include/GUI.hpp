#pragma once
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_dx11.h>

#include <SDL2/SDL.h>

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
	};
}
