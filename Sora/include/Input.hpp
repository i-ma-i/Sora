#pragma once
#include "InputDevice.hpp"

namespace sora
{
	class Input
	{
	public:
		Input() = default;
		Input(InputDevice device, uint8_t code);

		/// @brief 入力デバイスのボタンが現在のフレームで押され始めたかを返します。
		/// @return 入力デバイスのボタンが現在のフレームで押され始めた場合 true, それ以外の場合は false
		bool Clicked() const;

		/// @brief 入力デバイスのボタンが押されているかを返します。
		/// @return 入力デバイスのボタンが押されている場合 true, それ以外の場合は false
		bool Pressed() const;

		/// @brief 入力デバイスのボタンが現在のフレームで離されたかを返します。
		/// @return 入力デバイスのボタンが現在のフレームで離された場合 true, それ以外の場合は false
		bool Released() const;

	private:
		InputDevice m_device = InputDevice::Undefined;
		uint8_t m_code = 0;
	};
}
