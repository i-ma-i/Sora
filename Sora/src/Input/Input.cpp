#include "Input.hpp"
#include "Engine.hpp"
#include "IKeyboard.hpp"
#include "IMouse.hpp"

namespace sora
{
	Input::Input(InputDevice device, uint8_t code) :
		m_device(device),
		m_code(code)
	{}

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
}
