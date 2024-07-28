#pragma once
#include "Keyboard.hpp"
#include "MouseInput.hpp"

namespace
{
	static DirectX::SimpleMath::Matrix CreatePerspective(float fov, float aspectRate, float nearZ, float farZ)
	{
		return DirectX::XMMatrixPerspectiveFovLH(fov, aspectRate, nearZ, farZ);
	}

	static DirectX::SimpleMath::Matrix CreateOrthographic(float width, float height, float nearZ, float farZ)
	{
		return DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
	}
}

namespace
{
	constexpr float MOVE_SCALE = 10.0f;
	constexpr float ROTATE_SCALE = 0.1f;
	constexpr float ZOOM_SCALE = 20.0f;

	using namespace sora;
	Input s_inputForward = Keyboard::KEY_W;
	Input s_inputBackward = Keyboard::KEY_S;
	Input s_inputLeft = Keyboard::KEY_A;
	Input s_inputRight = Keyboard::KEY_D;
	Input s_inputUp = Keyboard::KEY_E;
	Input s_inputDown = Keyboard::KEY_Q;
	MouseInput s_mouseInput;
}

namespace sora
{
	class Camera
	{
	public:
		Camera()
		: m_position(DirectX::SimpleMath::Vector3(
			Config::GetFloat("Camera.m_position[0]"),
			Config::GetFloat("Camera.m_position[1]"),
			Config::GetFloat("Camera.m_position[2]")
		))
			, m_yawRad(Config::GetFloat("Camera.m_yawRad"))
			, m_pitchRad(Config::GetFloat("Camera.m_pitchRad"))
			, m_moveSpeed(Config::GetFloat("Camera.m_moveSpeed"))
			, m_Up(DirectX::SimpleMath::Vector3::Up)
		{
			m_projection = CreatePerspective(
				DirectX::XM_PIDIV4,
				16.0f / 9.0f,
				Config::GetFloat("Camera.m_nearZ"),
				Config::GetFloat("Camera.m_farZ")
			);
		}

		DirectX::SimpleMath::Matrix GetView() const { return m_view; }
		DirectX::SimpleMath::Matrix GetProjection() const { return m_projection; }
		DirectX::SimpleMath::Matrix GetViewProjection() const { return m_view * m_projection;}

		void Update(float deltaTime)
		{
			using namespace DirectX;
			using namespace DirectX::SimpleMath;

			const Vector3 forward = GetForwardLH();
			const Vector3 right = Vector3::Up.Cross(forward);
			const float deltaMove = m_moveSpeed * deltaTime * MOVE_SCALE;
			const Vector2 mouseDelta = Vector2((float)s_mouseInput.DeltaX(), (float)s_mouseInput.DeltaY()) * deltaTime;

			// マウスホイールでズームする。
			m_position += forward * (float)s_mouseInput.WheelValue() * deltaTime * ZOOM_SCALE;

			// マウス中ボタンを押している間、カメラをパン移動する。
			if (s_mouseInput.MiddlePressed())
			{
				m_position += right * mouseDelta.x * deltaMove * 5.0f;
				m_position += Vector3::Down * mouseDelta.y * deltaMove * 5.0f;
			}

			if (s_mouseInput.RightPressed())
			{
				// マウスの右ボタンを押している間、カメラを回転させる。
				m_yawRad += mouseDelta.x * ROTATE_SCALE;
				m_pitchRad -= mouseDelta.y * ROTATE_SCALE;
				m_pitchRad = std::clamp(m_pitchRad, -XM_PIDIV2, XM_PIDIV2);

				// マウスの右ボタンを押している間,カメラを移動させる。
				if (s_inputForward.Pressed()) m_position += forward * deltaMove;
				if (s_inputBackward.Pressed()) m_position -= forward * deltaMove;
				if (s_inputLeft.Pressed()) m_position -= right * deltaMove;
				if (s_inputRight.Pressed()) m_position += right * deltaMove;
				if (s_inputUp.Pressed()) m_position += Vector3::Up * deltaMove;
				if (s_inputDown.Pressed()) m_position += Vector3::Down * deltaMove;
			}

			// TODO: ビュー行列が変換された時だけ計算する。
			m_view = XMMatrixLookToLH(m_position, GetForwardLH(), m_Up);
		}

	private:
		DirectX::SimpleMath::Vector3 GetForwardLH() const
		{
			return {
				cosf(m_pitchRad) * sinf(m_yawRad),
				sinf(m_pitchRad),
				cosf(m_pitchRad) * cosf(m_yawRad)
			};
		}

	private:
		DirectX::SimpleMath::Vector3 m_position;
		float m_yawRad, m_pitchRad;
		float m_moveSpeed;

	private:
		DirectX::SimpleMath::Vector3 m_Up;
		DirectX::SimpleMath::Matrix m_view, m_projection;

	private:
		friend class GUI;
	};
}
