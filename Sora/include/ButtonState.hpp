#pragma once

namespace sora
{
	class ButtonState
	{
	public:
		void Update(bool currentPressed)
		{
			const bool previousPressed = m_pressed;
			m_pressed = currentPressed;
			m_clicked = (!previousPressed && m_pressed);
			m_released = (previousPressed && !m_pressed);
		}

		bool Clicked() const noexcept
		{
			return m_clicked;
		}

		bool Pressed() const noexcept
		{
			return m_pressed;
		}

		bool Released() const noexcept
		{
			return m_released;
		}

	private:
		bool m_clicked = false;
		bool m_pressed = false;
		bool m_released = false;
	};
}
