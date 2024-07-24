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
		void Update(int wheel) override;

		bool Clicked(std::uint8_t index) const override;
		bool Pressed(std::uint8_t index) const override;
		bool Released(std::uint8_t index) const override;

		int PositionX() const override;
		int PositionY() const override;
		int DeltaX() const override;
		int DeltaY() const override;
		int WheelValue() const override;

	private:
		std::array<ButtonState, ButtonCount + 1> m_state;
		int m_positionX = 0;
		int m_positionY = 0;
		int m_deltaX = 0;
		int m_deltaY = 0;
		int m_wheelValue = 0;
	};
}
