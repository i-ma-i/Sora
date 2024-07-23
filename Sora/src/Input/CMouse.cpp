#include "CMouse.hpp"

#include <SDL2/SDL_mouse.h>

namespace sora
{
	void CMouse::Update()
	{
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);

		for (int i = 1; i <= ButtonCount; i++)
		{
			m_state[i].Update(buttons & SDL_BUTTON(i));
		}
	}

	bool CMouse::Clicked(std::uint8_t index) const
	{
		assert(index <= ButtonCount);
		return m_state[index].Clicked();
	}

	bool CMouse::Pressed(std::uint8_t index) const
	{
		assert(index <= ButtonCount);
		return m_state[index].Pressed();
	}

	bool CMouse::Released(std::uint8_t index) const
	{
		assert(index <= ButtonCount);
		return m_state[index].Released();
	}
}
