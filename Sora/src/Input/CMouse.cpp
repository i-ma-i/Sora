#include "CMouse.hpp"

#include <SDL2/SDL_mouse.h>

namespace sora
{
	void CMouse::Update(int wheel)
	{
		Uint32 buttons = SDL_GetMouseState(&m_positionX, &m_positionY);
		SDL_GetRelativeMouseState(&m_deltaX, &m_deltaY);
		m_wheelValue = wheel;

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

	int CMouse::PositionX() const
	{
		return m_positionX;
	}
	int CMouse::PositionY() const
	{
		return m_positionY;
	}
	int CMouse::DeltaX() const
	{
		return m_deltaX;
	}
	int CMouse::DeltaY() const
	{
		return m_deltaY;
	}
	int CMouse::WheelValue() const
	{
		return m_wheelValue;
	}
}
