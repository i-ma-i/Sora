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
		void Update() override;

		bool Clicked(std::uint8_t index) const override;
		bool Pressed(std::uint8_t index) const override;
		bool Released(std::uint8_t index) const override;

	private:
		std::array<ButtonState, ButtonCount + 1> m_state;
	};
}
