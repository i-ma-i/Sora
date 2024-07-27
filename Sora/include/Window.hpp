#pragma once

struct SDL_Window;

namespace sora
{
	class Window
	{
	public:
		Window();
		~Window();

		HWND GetHWND() const { return m_hWnd; }

	private:
		SDL_Window* m_window;
		HWND m_hWnd;

	private:
		friend class GUI;
	};
}
