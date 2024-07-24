#pragma once

namespace sora
{
	class IMouse
	{
	public:
		virtual ~IMouse() = default;

		virtual void Update(int wheel) = 0;

		virtual bool Clicked(std::uint8_t index) const = 0;
		virtual bool Pressed(std::uint8_t index) const = 0;
		virtual bool Released(std::uint8_t index) const = 0;

		virtual int PositionX() const = 0;
		virtual int PositionY() const = 0;
		virtual int DeltaX() const = 0;
		virtual int DeltaY() const = 0;
		virtual int WheelValue() const = 0;
	};
}
