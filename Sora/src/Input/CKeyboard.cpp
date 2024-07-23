#include "CKeyboard.hpp"

#include <SDL2/SDL_keyboard.h>

namespace sora
{
	void CKeyboard::Update()
	{
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		for (int i = 0; i < KeyCount; i++)
		{
			m_state[i].Update(state[i]);
		}
	}

	bool CKeyboard::Clicked(std::uint8_t index) const
	{
		assert(index < KeyCount);
		return m_state[index].Clicked();
	}

	bool CKeyboard::Pressed(std::uint8_t index) const
	{
		assert(index < KeyCount);
		return m_state[index].Pressed();
	}

	bool CKeyboard::Released(std::uint8_t index) const
	{
		assert(index < KeyCount);
		return m_state[index].Released();
	}
}
