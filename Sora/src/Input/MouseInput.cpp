#include "MouseInput.hpp"
#include "Engine.hpp"
#include "IMouse.hpp"
#include "Mouse.hpp"

namespace sora
{
	bool MouseInput::LeftClicked() const
	{
		return Mouse::LEFT.Clicked();
	}
	bool MouseInput::LeftPressed() const
	{
		return Mouse::LEFT.Pressed();
	}
	bool MouseInput::LeftReleased() const
	{
		return Mouse::LEFT.Released();
	}
	bool MouseInput::RightClicked() const
	{
		return Mouse::RIGHT.Clicked();
	}
	bool MouseInput::RightPressed() const
	{
		return Mouse::RIGHT.Pressed();
	}
	bool MouseInput::RightReleased() const
	{
		return Mouse::RIGHT.Released();
	}
	bool MouseInput::MiddleClicked() const
	{
		return Mouse::MIDDLE.Clicked();
	}
	bool MouseInput::MiddlePressed() const
	{
		return Mouse::MIDDLE.Pressed();
	}
	bool MouseInput::MiddleReleased() const
	{
		return Mouse::MIDDLE.Released();
	}
	int MouseInput::PositionX() const
	{
		return Engine::GetModule<IMouse>().PositionX();
	}
	int MouseInput::PositionY() const
	{
		return Engine::GetModule<IMouse>().PositionY();
	}
	int MouseInput::DeltaX() const
	{
		return Engine::GetModule<IMouse>().DeltaX();
	}
	int MouseInput::DeltaY() const
	{
		return Engine::GetModule<IMouse>().DeltaY();
	}
	int MouseInput::WheelValue() const
	{
		return Engine::GetModule<IMouse>().WheelValue();
	}
}
