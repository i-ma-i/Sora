#pragma once

namespace sora
{
	enum class InputDevice
	{
		Undefined,
		Keyboard,
		Mouse,
	};

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

	class Input
	{
	public:
		Input() = default;
		Input(InputDevice device, uint8_t code);

		bool Clicked() const;

		bool Pressed() const;

		bool Released() const;

	private:
		InputDevice m_device = InputDevice::Undefined;
		uint8_t m_code = 0;
	};

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

	namespace Mouse
	{
		inline const Input LEFT{ InputDevice::Mouse, 1 };
		inline const Input MIDDLE{ InputDevice::Mouse, 2 };
		inline const Input RIGHT{ InputDevice::Mouse, 3 };
		inline const Input BACK{ InputDevice::Mouse, 4 };
		inline const Input FORWARD{ InputDevice::Mouse, 5 };
	}

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

	class CMouse : public IMouse
	{
	public:
		static constexpr std::uint8_t ButtonCount = 8;

	public:
		bool Clicked(std::uint8_t index) const override { return m_state.at(index).Clicked(); }
		bool Pressed(std::uint8_t index) const override { return m_state.at(index).Pressed(); }
		bool Released(std::uint8_t index) const override { return m_state.at(index).Released(); }

		inline int PositionX() const override { return m_positionX; }
		inline int PositionY() const override { return m_positionY; }
		inline int DeltaX() const override { return m_deltaX; }
		inline int DeltaY() const override { return m_deltaY; }
		inline int WheelValue() const override { return m_wheelValue; }

	private:
		void Update(int wheel) override;

	private:
		std::array<ButtonState, ButtonCount + 1> m_state;
		int m_positionX = 0;
		int m_positionY = 0;
		int m_deltaX = 0;
		int m_deltaY = 0;
		int m_wheelValue = 0;
	};

	namespace Keyboard
	{
		inline const Input KEY_A{ InputDevice::Keyboard, 4 };
		inline const Input KEY_B{ InputDevice::Keyboard, 5 };
		inline const Input KEY_C{ InputDevice::Keyboard, 6 };
		inline const Input KEY_D{ InputDevice::Keyboard, 7 };
		inline const Input KEY_E{ InputDevice::Keyboard, 8 };
		inline const Input KEY_F{ InputDevice::Keyboard, 9 };
		inline const Input KEY_G{ InputDevice::Keyboard, 10 };
		inline const Input KEY_H{ InputDevice::Keyboard, 11 };
		inline const Input KEY_I{ InputDevice::Keyboard, 12 };
		inline const Input KEY_J{ InputDevice::Keyboard, 13 };
		inline const Input KEY_K{ InputDevice::Keyboard, 14 };
		inline const Input KEY_L{ InputDevice::Keyboard, 15 };
		inline const Input KEY_M{ InputDevice::Keyboard, 16 };
		inline const Input KEY_N{ InputDevice::Keyboard, 17 };
		inline const Input KEY_O{ InputDevice::Keyboard, 18 };
		inline const Input KEY_P{ InputDevice::Keyboard, 19 };
		inline const Input KEY_Q{ InputDevice::Keyboard, 20 };
		inline const Input KEY_R{ InputDevice::Keyboard, 21 };
		inline const Input KEY_S{ InputDevice::Keyboard, 22 };
		inline const Input KEY_T{ InputDevice::Keyboard, 23 };
		inline const Input KEY_U{ InputDevice::Keyboard, 24 };
		inline const Input KEY_V{ InputDevice::Keyboard, 25 };
		inline const Input KEY_W{ InputDevice::Keyboard, 26 };
		inline const Input KEY_X{ InputDevice::Keyboard, 27 };
		inline const Input KEY_Y{ InputDevice::Keyboard, 28 };
		inline const Input KEY_Z{ InputDevice::Keyboard, 29 };

		inline const Input KEY_1{ InputDevice::Keyboard, 30 };
		inline const Input KEY_2{ InputDevice::Keyboard, 31 };
		inline const Input KEY_3{ InputDevice::Keyboard, 32 };
		inline const Input KEY_4{ InputDevice::Keyboard, 33 };
		inline const Input KEY_5{ InputDevice::Keyboard, 34 };
		inline const Input KEY_6{ InputDevice::Keyboard, 35 };
		inline const Input KEY_7{ InputDevice::Keyboard, 36 };
		inline const Input KEY_8{ InputDevice::Keyboard, 37 };
		inline const Input KEY_9{ InputDevice::Keyboard, 38 };
		inline const Input KEY_0{ InputDevice::Keyboard, 39 };

		inline const Input KEY_RETURN{ InputDevice::Keyboard, 40 };
		inline const Input KEY_ESCAPE{ InputDevice::Keyboard, 41 };
		inline const Input KEY_BACKSPACE{ InputDevice::Keyboard, 42 };
		inline const Input KEY_TAB{ InputDevice::Keyboard, 43 };
		inline const Input KEY_SPACE{ InputDevice::Keyboard, 44 };

		inline const Input KEY_MINUS{ InputDevice::Keyboard, 45 };
		inline const Input KEY_EQUALS{ InputDevice::Keyboard, 46 };
		inline const Input KEY_LEFTBRACKET{ InputDevice::Keyboard, 47 };
		inline const Input KEY_RIGHTBRACKET{ InputDevice::Keyboard, 48 };
		inline const Input KEY_BACKSLASH{ InputDevice::Keyboard, 49 }; /**< Located at the lower left of the return
										*   key on ISO keyboards and at the right end
										*   of the QWERTY row on ANSI keyboards.
										*   Produces REVERSE SOLIDUS (backslash) and
										*   VERTICAL LINE in a US layout, REVERSE
										*   SOLIDUS and VERTICAL LINE in a UK Mac
										*   layout, NUMBER SIGN and TILDE in a UK
										*   Windows layout, DOLLAR SIGN and POUND SIGN
										*   in a Swiss German layout, NUMBER SIGN and
										*   APOSTROPHE in a German layout, GRAVE
										*   ACCENT and POUND SIGN in a French Mac
										*   layout, and ASTERISK and MICRO SIGN in a
										*   French Windows layout.
										*/
		inline const Input KEY_NONUSHASH{ InputDevice::Keyboard, 50 }; /**< ISO USB keyboards actually use this code
										*   instead of 49 for the same key, but all
										*   OSes I've seen treat the two codes
										*   identically. So, as an implementor, unless
										*   your keyboard generates both of those
										*   codes and your OS treats them differently,
										*   you should generate inline const Input KEY_BACKSLASH
										*   instead of this code. As a user, you
										*   should not rely on this code because SDL
										*   will never generate it with most (all?)
										*   keyboards.
										*/
		inline const Input KEY_SEMICOLON{ InputDevice::Keyboard, 51 };
		inline const Input KEY_APOSTROPHE{ InputDevice::Keyboard, 52 };
		inline const Input KEY_GRAVE{ InputDevice::Keyboard, 53 }; /**< Located in the top left corner (on both ANSI
									*   and ISO keyboards). Produces GRAVE ACCENT and
									*   TILDE in a US Windows layout and in US and UK
									*   Mac layouts on ANSI keyboards, GRAVE ACCENT
									*   and NOT SIGN in a UK Windows layout, SECTION
									*   SIGN and PLUS-MINUS SIGN in US and UK Mac
									*   layouts on ISO keyboards, SECTION SIGN and
									*   DEGREE SIGN in a Swiss German layout (Mac:
									*   only on ISO keyboards), CIRCUMFLEX ACCENT and
									*   DEGREE SIGN in a German layout (Mac: only on
									*   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
									*   French Windows layout, COMMERCIAL AT and
									*   NUMBER SIGN in a French Mac layout on ISO
									*   keyboards, and LESS-THAN SIGN and GREATER-THAN
									*   SIGN in a Swiss German, German, or French Mac
									*   layout on ANSI keyboards.
									*/
		inline const Input KEY_COMMA{ InputDevice::Keyboard, 54 };
		inline const Input KEY_PERIOD{ InputDevice::Keyboard, 55 };
		inline const Input KEY_SLASH{ InputDevice::Keyboard, 56 };

		inline const Input KEY_CAPSLOCK{ InputDevice::Keyboard, 57 };

		inline const Input KEY_F1{ InputDevice::Keyboard, 58 };
		inline const Input KEY_F2{ InputDevice::Keyboard, 59 };
		inline const Input KEY_F3{ InputDevice::Keyboard, 60 };
		inline const Input KEY_F4{ InputDevice::Keyboard, 61 };
		inline const Input KEY_F5{ InputDevice::Keyboard, 62 };
		inline const Input KEY_F6{ InputDevice::Keyboard, 63 };
		inline const Input KEY_F7{ InputDevice::Keyboard, 64 };
		inline const Input KEY_F8{ InputDevice::Keyboard, 65 };
		inline const Input KEY_F9{ InputDevice::Keyboard, 66 };
		inline const Input KEY_F10{ InputDevice::Keyboard, 67 };
		inline const Input KEY_F11{ InputDevice::Keyboard, 68 };
		inline const Input KEY_F12{ InputDevice::Keyboard, 69 };

		inline const Input KEY_PRINTSCREEN{ InputDevice::Keyboard, 70 };
		inline const Input KEY_SCROLLLOCK{ InputDevice::Keyboard, 71 };
		inline const Input KEY_PAUSE{ InputDevice::Keyboard, 72 };
		inline const Input KEY_INSERT{ InputDevice::Keyboard, 73 }; /**< insert on PC, help on some Mac keyboards (but
										does send code 73 }; not 117) */
		inline const Input KEY_HOME{ InputDevice::Keyboard, 74 };
		inline const Input KEY_PAGEUP{ InputDevice::Keyboard, 75 };
		inline const Input KEY_DELETE{ InputDevice::Keyboard, 76 };
		inline const Input KEY_END{ InputDevice::Keyboard, 77 };
		inline const Input KEY_PAGEDOWN{ InputDevice::Keyboard, 78 };
		inline const Input KEY_RIGHT{ InputDevice::Keyboard, 79 };
		inline const Input KEY_LEFT{ InputDevice::Keyboard, 80 };
		inline const Input KEY_DOWN{ InputDevice::Keyboard, 81 };
		inline const Input KEY_UP{ InputDevice::Keyboard, 82 };

		inline const Input KEY_NUMLOCKCLEAR{ InputDevice::Keyboard, 83 }; /**< num lock on PC, clear on Mac keyboards
											*/
		inline const Input KEY_KP_DIVIDE{ InputDevice::Keyboard, 84 };
		inline const Input KEY_KP_MULTIPLY{ InputDevice::Keyboard, 85 };
		inline const Input KEY_KP_MINUS{ InputDevice::Keyboard, 86 };
		inline const Input KEY_KP_PLUS{ InputDevice::Keyboard, 87 };
		inline const Input KEY_KP_ENTER{ InputDevice::Keyboard, 88 };
		inline const Input KEY_KP_1{ InputDevice::Keyboard, 89 };
		inline const Input KEY_KP_2{ InputDevice::Keyboard, 90 };
		inline const Input KEY_KP_3{ InputDevice::Keyboard, 91 };
		inline const Input KEY_KP_4{ InputDevice::Keyboard, 92 };
		inline const Input KEY_KP_5{ InputDevice::Keyboard, 93 };
		inline const Input KEY_KP_6{ InputDevice::Keyboard, 94 };
		inline const Input KEY_KP_7{ InputDevice::Keyboard, 95 };
		inline const Input KEY_KP_8{ InputDevice::Keyboard, 96 };
		inline const Input KEY_KP_9{ InputDevice::Keyboard, 97 };
		inline const Input KEY_KP_0{ InputDevice::Keyboard, 98 };
		inline const Input KEY_KP_PERIOD{ InputDevice::Keyboard, 99 };

		inline const Input KEY_NONUSBACKSLASH{ InputDevice::Keyboard, 100 }; /**< This is the additional key that ISO
											*   keyboards have over ANSI ones,
											*   located between left shift and Y.
											*   Produces GRAVE ACCENT and TILDE in a
											*   US or UK Mac layout, REVERSE SOLIDUS
											*   (backslash) and VERTICAL LINE in a
											*   US or UK Windows layout, and
											*   LESS-THAN SIGN and GREATER-THAN SIGN
											*   in a Swiss German, German, or French
											*   layout. */
		inline const Input KEY_APPLICATION{ InputDevice::Keyboard, 101 }; /**< windows contextual menu, compose */
		inline const Input KEY_POWER{ InputDevice::Keyboard, 102 }; /**< The USB document says this is a status flag,
									*   not a physical key - but some Mac keyboards
									*   do have a power key. */
		inline const Input KEY_KP_EQUALS{ InputDevice::Keyboard, 103 };
		inline const Input KEY_F13{ InputDevice::Keyboard, 104 };
		inline const Input KEY_F14{ InputDevice::Keyboard, 105 };
		inline const Input KEY_F15{ InputDevice::Keyboard, 106 };
		inline const Input KEY_F16{ InputDevice::Keyboard, 107 };
		inline const Input KEY_F17{ InputDevice::Keyboard, 108 };
		inline const Input KEY_F18{ InputDevice::Keyboard, 109 };
		inline const Input KEY_F19{ InputDevice::Keyboard, 110 };
		inline const Input KEY_F20{ InputDevice::Keyboard, 111 };
		inline const Input KEY_F21{ InputDevice::Keyboard, 112 };
		inline const Input KEY_F22{ InputDevice::Keyboard, 113 };
		inline const Input KEY_F23{ InputDevice::Keyboard, 114 };
		inline const Input KEY_F24{ InputDevice::Keyboard, 115 };
		inline const Input KEY_EXECUTE1{ InputDevice::Keyboard, 116 };
		inline const Input KEY_HELP{ InputDevice::Keyboard, 117 };    /**< AL Integrated Help Center */
		inline const Input KEY_MENU{ InputDevice::Keyboard, 118 };    /**< Menu (show menu) */
		inline const Input KEY_SELECT{ InputDevice::Keyboard, 119 };
		inline const Input KEY_STOP{ InputDevice::Keyboard, 120 };    /**< AC Stop */
		inline const Input KEY_AGAIN{ InputDevice::Keyboard, 121 };   /**< AC Redo/Repeat */
		inline const Input KEY_UNDO{ InputDevice::Keyboard, 122 };    /**< AC Undo */
		inline const Input KEY_CUT{ InputDevice::Keyboard, 123 };     /**< AC Cut */
		inline const Input KEY_COPY{ InputDevice::Keyboard, 124 };    /**< AC Copy */
		inline const Input KEY_PASTE{ InputDevice::Keyboard, 125 };   /**< AC Paste */
		inline const Input KEY_FIND{ InputDevice::Keyboard, 126 };    /**< AC Find */
		inline const Input KEY_MUTE{ InputDevice::Keyboard, 127 };
		inline const Input KEY_VOLUMEUP{ InputDevice::Keyboard, 128 };
		inline const Input KEY_VOLUMEDOWN{ InputDevice::Keyboard, 129 };
		/* not sure whether there's a reason to enable these */
		/*     inline const Input KEY_LOCKINGCAPSLOCK{ InputDevice::Keyboard, 130 };  */
		/*     inline const Input KEY_LOCKINGNUMLOCK{ InputDevice::Keyboard, 131 }; */
		/*     inline const Input KEY_LOCKINGSCROLLLOCK{ InputDevice::Keyboard, 132 }; */
		inline const Input KEY_KP_COMMA{ InputDevice::Keyboard, 133 };
		inline const Input KEY_KP_EQUALSAS400{ InputDevice::Keyboard, 134 };

		inline const Input KEY_INTERNATIONAL1{ InputDevice::Keyboard, 135 }; /**< used on Asian keyboards, see
												footnotes in USB doc */
		inline const Input KEY_INTERNATIONAL2{ InputDevice::Keyboard, 136 };
		inline const Input KEY_INTERNATIONAL3{ InputDevice::Keyboard, 137 }; /**< Yen */
		inline const Input KEY_INTERNATIONAL4{ InputDevice::Keyboard, 138 };
		inline const Input KEY_INTERNATIONAL5{ InputDevice::Keyboard, 139 };
		inline const Input KEY_INTERNATIONAL6{ InputDevice::Keyboard, 140 };
		inline const Input KEY_INTERNATIONAL7{ InputDevice::Keyboard, 141 };
		inline const Input KEY_INTERNATIONAL8{ InputDevice::Keyboard, 142 };
		inline const Input KEY_INTERNATIONAL9{ InputDevice::Keyboard, 143 };
		inline const Input KEY_LANG1{ InputDevice::Keyboard, 144 }; /**< Hangul/English toggle */
		inline const Input KEY_LANG2{ InputDevice::Keyboard, 145 }; /**< Hanja conversion */
		inline const Input KEY_LANG3{ InputDevice::Keyboard, 146 }; /**< Katakana */
		inline const Input KEY_LANG4{ InputDevice::Keyboard, 147 }; /**< Hiragana */
		inline const Input KEY_LANG5{ InputDevice::Keyboard, 148 }; /**< Zenkaku/Hankaku */
		inline const Input KEY_LANG6{ InputDevice::Keyboard, 149 }; /**< reserved */
		inline const Input KEY_LANG7{ InputDevice::Keyboard, 150 }; /**< reserved */
		inline const Input KEY_LANG8{ InputDevice::Keyboard, 151 }; /**< reserved */
		inline const Input KEY_LANG9{ InputDevice::Keyboard, 152 }; /**< reserved */

		inline const Input KEY_ALTERASE{ InputDevice::Keyboard, 153 };    /**< Erase-Eaze */
		inline const Input KEY_SYSREQ{ InputDevice::Keyboard, 154 };
		inline const Input KEY_CANCEL{ InputDevice::Keyboard, 155 };      /**< AC Cancel */
		inline const Input KEY_CLEAR{ InputDevice::Keyboard, 156 };
		inline const Input KEY_PRIOR{ InputDevice::Keyboard, 157 };
		inline const Input KEY_RETURN2{ InputDevice::Keyboard, 158 };
		inline const Input KEY_SEPARATOR{ InputDevice::Keyboard, 159 };
		inline const Input KEY_OUT{ InputDevice::Keyboard, 160 };
		inline const Input KEY_OPER{ InputDevice::Keyboard, 161 };
		inline const Input KEY_CLEARAGAIN{ InputDevice::Keyboard, 162 };
		inline const Input KEY_CRSEL{ InputDevice::Keyboard, 163 };
		inline const Input KEY_EXSEL{ InputDevice::Keyboard, 164 };

		inline const Input KEY_KP_00{ InputDevice::Keyboard, 176 };
		inline const Input KEY_KP_000{ InputDevice::Keyboard, 177 };
		inline const Input KEY_THOUSANDSSEPARATOR{ InputDevice::Keyboard, 178 };
		inline const Input KEY_DECIMALSEPARATOR{ InputDevice::Keyboard, 179 };
		inline const Input KEY_CURRENCYUNIT{ InputDevice::Keyboard, 180 };
		inline const Input KEY_CURRENCYSUBUNIT{ InputDevice::Keyboard, 181 };
		inline const Input KEY_KP_LEFTPAREN{ InputDevice::Keyboard, 182 };
		inline const Input KEY_KP_RIGHTPAREN{ InputDevice::Keyboard, 183 };
		inline const Input KEY_KP_LEFTBRACE{ InputDevice::Keyboard, 184 };
		inline const Input KEY_KP_RIGHTBRACE{ InputDevice::Keyboard, 185 };
		inline const Input KEY_KP_TAB{ InputDevice::Keyboard, 186 };
		inline const Input KEY_KP_BACKSPACE{ InputDevice::Keyboard, 187 };
		inline const Input KEY_KP_A{ InputDevice::Keyboard, 188 };
		inline const Input KEY_KP_B{ InputDevice::Keyboard, 189 };
		inline const Input KEY_KP_C{ InputDevice::Keyboard, 190 };
		inline const Input KEY_KP_D{ InputDevice::Keyboard, 191 };
		inline const Input KEY_KP_E{ InputDevice::Keyboard, 192 };
		inline const Input KEY_KP_F{ InputDevice::Keyboard, 193 };
		inline const Input KEY_KP_XOR{ InputDevice::Keyboard, 194 };
		inline const Input KEY_KP_POWER{ InputDevice::Keyboard, 195 };
		inline const Input KEY_KP_PERCENT{ InputDevice::Keyboard, 196 };
		inline const Input KEY_KP_LESS{ InputDevice::Keyboard, 197 };
		inline const Input KEY_KP_GREATER{ InputDevice::Keyboard, 198 };
		inline const Input KEY_KP_AMPERSAND{ InputDevice::Keyboard, 199 };
		inline const Input KEY_KP_DBLAMPERSAND{ InputDevice::Keyboard, 200 };
		inline const Input KEY_KP_VERTICALBAR{ InputDevice::Keyboard, 201 };
		inline const Input KEY_KP_DBLVERTICALBAR{ InputDevice::Keyboard, 202 };
		inline const Input KEY_KP_COLON{ InputDevice::Keyboard, 203 };
		inline const Input KEY_KP_HASH{ InputDevice::Keyboard, 204 };
		inline const Input KEY_KP_SPACE{ InputDevice::Keyboard, 205 };
		inline const Input KEY_KP_AT{ InputDevice::Keyboard, 206 };
		inline const Input KEY_KP_EXCLAM{ InputDevice::Keyboard, 207 };
		inline const Input KEY_KP_MEMSTORE{ InputDevice::Keyboard, 208 };
		inline const Input KEY_KP_MEMRECALL{ InputDevice::Keyboard, 209 };
		inline const Input KEY_KP_MEMCLEAR{ InputDevice::Keyboard, 210 };
		inline const Input KEY_KP_MEMADD{ InputDevice::Keyboard, 211 };
		inline const Input KEY_KP_MEMSUBTRACT{ InputDevice::Keyboard, 212 };
		inline const Input KEY_KP_MEMMULTIPLY{ InputDevice::Keyboard, 213 };
		inline const Input KEY_KP_MEMDIVIDE{ InputDevice::Keyboard, 214 };
		inline const Input KEY_KP_PLUSMINUS{ InputDevice::Keyboard, 215 };
		inline const Input KEY_KP_CLEAR{ InputDevice::Keyboard, 216 };
		inline const Input KEY_KP_CLEARENTRY{ InputDevice::Keyboard, 217 };
		inline const Input KEY_KP_BINARY{ InputDevice::Keyboard, 218 };
		inline const Input KEY_KP_OCTAL{ InputDevice::Keyboard, 219 };
		inline const Input KEY_KP_DECIMAL{ InputDevice::Keyboard, 220 };
		inline const Input KEY_KP_HEXADECIMAL{ InputDevice::Keyboard, 221 };

		inline const Input KEY_LCTRL{ InputDevice::Keyboard, 224 };
		inline const Input KEY_LSHIFT{ InputDevice::Keyboard, 225 };
		inline const Input KEY_LALT{ InputDevice::Keyboard, 226 }; /**< alt, option */
		inline const Input KEY_LGUI{ InputDevice::Keyboard, 227 }; /**< windows, command (apple), meta */
		inline const Input KEY_RCTRL{ InputDevice::Keyboard, 228 };
		inline const Input KEY_RSHIFT{ InputDevice::Keyboard, 229 };
		inline const Input KEY_RALT{ InputDevice::Keyboard, 230 }; /**< alt gr, option */
		inline const Input KEY_RGUI{ InputDevice::Keyboard, 231 };
	}

	class IKeyboard
	{
	public:
		virtual ~IKeyboard() = default;

		virtual void Update() = 0;

		virtual bool Clicked(std::uint8_t index) const = 0;
		virtual bool Pressed(std::uint8_t index) const = 0;
		virtual bool Released(std::uint8_t index) const = 0;
	};

	class CKeyboard : public IKeyboard
	{
	public:
		static constexpr std::uint16_t KeyCount = 256;

	public:
		void Update() override;

		bool Clicked(std::uint8_t index) const override { return m_state.at(index).Clicked(); }
		bool Pressed(std::uint8_t index) const override { return m_state.at(index).Pressed(); }
		bool Released(std::uint8_t index) const override { return m_state.at(index).Released(); }

	private:
		std::array<ButtonState, KeyCount> m_state;
	};
}
