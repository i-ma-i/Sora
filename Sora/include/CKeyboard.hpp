#pragma once
#include "IKeyboard.hpp"
#include "ButtonState.hpp"

namespace sora
{
	class CKeyboard : public IKeyboard
	{
	public:
		static constexpr std::uint16_t KeyCount = 256;

	public:
		void Update() override
		{
			const Uint8* state = SDL_GetKeyboardState(nullptr);
			for (int i = 0; i < KeyCount; i++)
			{
				m_state[i].Update(state[i]);
			}
		}

		bool Clicked(std::uint8_t index) const override { return m_state.at(index).Clicked(); }
		bool Pressed(std::uint8_t index) const override { return m_state.at(index).Pressed(); }
		bool Released(std::uint8_t index) const override { return m_state.at(index).Released(); }

	private:
		std::array<ButtonState, KeyCount> m_state;
	};
}
