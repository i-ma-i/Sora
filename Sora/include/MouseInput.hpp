#pragma once

namespace sora
{
	class MouseInput
	{
	public:
		bool LeftClicked() const;
		bool LeftPressed() const;
		bool LeftReleased() const;

		bool RightClicked() const;
		bool RightPressed() const;
		bool RightReleased() const;

		bool MiddleClicked() const;
		bool MiddlePressed() const;
		bool MiddleReleased() const;

		int PositionX() const;
		int PositionY() const;
		int DeltaX() const;
		int DeltaY() const;

		int WheelValue() const;
	};
}
