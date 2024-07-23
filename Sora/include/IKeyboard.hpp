#pragma once

namespace sora
{
	class IKeyboard
	{
	public:
		virtual ~IKeyboard() = default;

		virtual void Update() = 0;

		virtual bool Clicked(std::uint8_t index) const = 0;
		virtual bool Pressed(std::uint8_t index) const = 0;
		virtual bool Released(std::uint8_t index) const = 0;
	};
}
