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
			Config::GetFloat("camera.position[0]"),
			Config::GetFloat("camera.position[1]"),
			Config::GetFloat("camera.position[2]")
		))
		, m_yawRad(Config::GetFloat("camera.yaw"))
		, m_pitchRad(Config::GetFloat("camera.pitch"))
		, m_moveSpeed(Config::GetFloat("camera.moveSpeed"))
		, m_rotateSpeedRad(Config::GetFloat("camera.rotateSpeed"))
		, m_zoomSpeed(Config::GetFloat("camera.zoomSpeed"))
		{
			m_projection = CreatePerspective(
				DirectX::XM_PIDIV4,
				16.0f / 9.0f,
				Config::GetFloat("camera.nearZ"),
				Config::GetFloat("camera.farZ")
			);
		}

		DirectX::SimpleMath::Matrix GetView() const { return DirectX::XMMatrixLookToLH(m_position, GetForward(), DirectX::SimpleMath::Vector3::Up); }
		DirectX::SimpleMath::Matrix GetProjection() const { return m_projection; }
		DirectX::SimpleMath::Matrix GetViewProjection() const { return GetView() * m_projection;}

		void Update(float deltaTime)
		{
			using namespace DirectX;
			using namespace DirectX::SimpleMath;

			const Vector3 forward = GetForward();
			const Vector3 right = Vector3::Up.Cross(forward);
			const float deltaMove = m_moveSpeed * deltaTime;
			const Vector2 mouseDelta = Vector2((float)s_mouseInput.DeltaX(), (float)s_mouseInput.DeltaY()) * deltaTime;

			// マウスホイールでズームする。
			m_position += forward * (float)s_mouseInput.WheelValue() * m_zoomSpeed * deltaTime;

			// マウス中ボタンを押している間、カメラをパン移動する。
			if (s_mouseInput.MiddlePressed())
			{
				m_position += right * mouseDelta.x * deltaMove * 5.0f;
				m_position += Vector3::Down * mouseDelta.y * deltaMove * 5.0f;
			}

			if (s_mouseInput.RightPressed() == false)
				return;

			// マウスの右ボタンを押している間、カメラを回転させる。
			m_yawRad += mouseDelta.x * m_rotateSpeedRad;
			m_pitchRad -= mouseDelta.y * m_rotateSpeedRad;
			m_pitchRad = std::clamp(m_pitchRad, -XM_PIDIV2, XM_PIDIV2);

			// マウスの右ボタンを押している間,カメラを移動させる。
			if (s_inputForward.Pressed()) m_position += forward * deltaMove;
			if (s_inputBackward.Pressed()) m_position -= forward * deltaMove;
			if (s_inputLeft.Pressed()) m_position -= right * deltaMove;
			if (s_inputRight.Pressed()) m_position += right * deltaMove;
			if (s_inputUp.Pressed()) m_position += Vector3::Up * deltaMove;
			if (s_inputDown.Pressed()) m_position += Vector3::Down * deltaMove;
		}

	private:
		DirectX::SimpleMath::Vector3 GetForward() const
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
		float m_moveSpeed, m_rotateSpeedRad, m_zoomSpeed;

	private:
		DirectX::SimpleMath::Matrix m_projection;

	private:
		friend class GUI;
	};
}
