﻿#include "Input.hpp"
#include "Engine.hpp"

namespace sora
{
	bool Input::Clicked() const
	{
		switch (m_device)
		{
		case InputDevice::Keyboard:
			return Engine::GetModule<IKeyboard>()->Clicked(m_code);
		case InputDevice::Mouse:
			return Engine::GetModule<IMouse>()->Clicked(m_code);
		default:
			return false;
		}
	}

	bool Input::Pressed() const
	{
		switch (m_device)
		{
		case InputDevice::Keyboard:
			return Engine::GetModule<IKeyboard>()->Pressed(m_code);
		case InputDevice::Mouse:
			return Engine::GetModule<IMouse>()->Pressed(m_code);
		default:
			return false;
		}
	}

	bool Input::Released() const
	{
		switch (m_device)
		{
		case InputDevice::Keyboard:
			return Engine::GetModule<IKeyboard>()->Released(m_code);
		case InputDevice::Mouse:
			return Engine::GetModule<IMouse>()->Released(m_code);
		default:
			return false;
		}
	}

	int MouseInput::PositionX() const
	{
		return Engine::GetModule<IMouse>()->PositionX();
	}
	int MouseInput::PositionY() const
	{
		return Engine::GetModule<IMouse>()->PositionY();
	}
	int MouseInput::DeltaX() const
	{
		return Engine::GetModule<IMouse>()->DeltaX();
	}
	int MouseInput::DeltaY() const
	{
		return Engine::GetModule<IMouse>()->DeltaY();
	}
	int MouseInput::WheelValue() const
	{
		return Engine::GetModule<IMouse>()->WheelValue();
	}

	void CMouse::Update(int wheel)
	{
		m_deltaX = 0;
		m_deltaY = 0;
		m_wheelValue = 0;

		Uint32 buttons = SDL_GetMouseState(&m_positionX, &m_positionY);
		SDL_GetRelativeMouseState(&m_deltaX, &m_deltaY);
		m_wheelValue = wheel;

		for (int i = 1; i <= ButtonCount; i++)
		{
			m_state[i].Update(buttons & SDL_BUTTON(i));
		}
	}

	void CKeyboard::Update()
	{
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		for (int i = 0; i < KeyCount; i++)
		{
			m_state[i].Update(state[i]);
		}
	}
}
