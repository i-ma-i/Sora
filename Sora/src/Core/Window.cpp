﻿#include "Window.hpp"

#include <SDL2/SDL_syswm.h>

namespace sora
{
	Window::Window()
	{
		m_window = SDL_CreateWindow(
			Config::GetString("window.title").c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			Config::GetInt("window.width"), Config::GetInt("window.height"),
			SDL_WINDOW_ALLOW_HIGHDPI
		);
		if (m_window == nullptr)
		{
			LOG_ERROR("[SDL] SDL_CreateWindow Error.  error: {}", SDL_GetError());
			DebugBreak();
		}

		// SDLウィンドウからハンドルを取得
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(m_window, &wmInfo);
		m_hWnd = (HWND)wmInfo.info.win.window;
	}

	Window::~Window()
	{
		SDL_DestroyWindow(m_window);
	}
}