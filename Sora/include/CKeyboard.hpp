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
		void Update() override;

		bool Clicked(std::uint8_t index) const override;
		bool Pressed(std::uint8_t index) const override;
		bool Released(std::uint8_t index) const override;

	private:
		std::array<ButtonState, KeyCount> m_state;
	};
}
