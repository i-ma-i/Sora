#pragma once
#include "IMouse.hpp"
#include "ButtonState.hpp"

namespace sora
{
	class CMouse : public IMouse
	{
	public:
		static constexpr std::uint8_t ButtonCount = 8;

	public:
		bool Clicked(std::uint8_t index) const override { return m_state.at(index).Clicked(); }
		bool Pressed(std::uint8_t index) const override { return m_state.at(index).Pressed(); }
		bool Released(std::uint8_t index) const override { return m_state.at(index).Released(); }

		inline int PositionX() const override { return m_positionX;}
		inline int PositionY() const override { return m_positionY;}
		inline int DeltaX() const override { return m_deltaX;}
		inline int DeltaY() const override { return m_deltaY;}
		inline int WheelValue() const override { return m_wheelValue; }

	private:
		void Update(int wheel) override
		{
			Uint32 buttons = SDL_GetMouseState(&m_positionX, &m_positionY);
			SDL_GetRelativeMouseState(&m_deltaX, &m_deltaY);
			m_wheelValue = wheel;

			for (int i = 1; i <= ButtonCount; i++)
			{
				m_state[i].Update(buttons & SDL_BUTTON(i));
			}
		}

	private:
		std::array<ButtonState, ButtonCount + 1> m_state;
		int m_positionX = 0;
		int m_positionY = 0;
		int m_deltaX = 0;
		int m_deltaY = 0;
		int m_wheelValue = 0;

	private:
		friend class Application;
	};
}
