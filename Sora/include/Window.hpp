#pragma once

namespace sora
{
	class Window
	{
	public:
		Window();
		~Window();

		inline HWND GetHWND() const { return m_hWnd; }

	private:
		SDL_Window* m_window = nullptr;
		HWND m_hWnd = 0;

	private:
		friend class GUI;
	};
}
