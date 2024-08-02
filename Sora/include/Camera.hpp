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
			: m_position(Config::GetVector3("Camera.m_position"))
			, m_target(Config::GetVector3("Camera.m_target"))
			, m_moveSpeed(Config::GetFloat("Camera.m_moveSpeed"))
		{
			m_projection = CreatePerspective(
				DirectX::XM_PIDIV4,
				16.0f / 9.0f,
				Config::GetFloat("Camera.m_nearZ"),
				Config::GetFloat("Camera.m_farZ")
			);

			// yawとpitchを計算する。
			DirectX::SimpleMath::Vector3 direction = m_target - m_position;
			direction.Normalize();
			m_pitch = asinf(direction.y);
			m_yaw = atan2f(direction.x, direction.z);
		}

		DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
		DirectX::SimpleMath::Matrix GetView() const { return m_view; }
		DirectX::SimpleMath::Matrix GetProjection() const { return m_projection; }
		DirectX::SimpleMath::Matrix GetViewProjection() const { return m_view * m_projection;}

		void Update(float deltaTime)
		{
			using namespace DirectX;
			using namespace DirectX::SimpleMath;

			m_forward = GetForwardLH();
			m_right = Vector3::Up.Cross(m_forward);
			const float deltaMove = m_moveSpeed * deltaTime * MOVE_SCALE;
			const Vector2 mouseDelta = Vector2((float)s_mouseInput.DeltaX(), (float)s_mouseInput.DeltaY()) * deltaTime * ROTATE_SCALE;

			// マウスホイールでズームする。
			m_position += m_forward * (float)s_mouseInput.WheelValue() * deltaTime * ZOOM_SCALE;

			if (s_mouseInput.RightPressed())
			{
				// マウスの右ボタンを押している間、カメラを回転させる。
				m_yaw += mouseDelta.x;
				m_pitch -= mouseDelta.y;
				m_pitch = std::clamp(m_pitch, -XM_PIDIV2, XM_PIDIV2);

				// マウスの右ボタンを押している間,カメラを移動させる。
				if (s_inputForward.Pressed()) m_position += m_forward * deltaMove;
				if (s_inputBackward.Pressed()) m_position -= m_forward * deltaMove;
				if (s_inputLeft.Pressed()) m_position -= m_right * deltaMove;
				if (s_inputRight.Pressed()) m_position += m_right * deltaMove;
				if (s_inputUp.Pressed()) m_position += Vector3::Up * deltaMove;
				if (s_inputDown.Pressed()) m_position += Vector3::Down * deltaMove;
			}

			m_target = m_position + m_forward;
			m_view = XMMatrixLookAtLH(m_position, m_target, Vector3::Up);
		}

	private:
		DirectX::SimpleMath::Vector3 GetForwardLH() const
		{
			return {
				cosf(m_pitch) * sinf(m_yaw),
				sinf(m_pitch),
				cosf(m_pitch) * cosf(m_yaw)
			};
		}

	private:
		DirectX::SimpleMath::Vector3 m_position, m_target;
		float m_yaw, m_pitch;
		float m_moveSpeed;

	private:
		DirectX::SimpleMath::Vector3 m_forward, m_right, m_up{ DirectX::SimpleMath::Vector3::Up };
		DirectX::SimpleMath::Matrix m_view, m_projection;

	private:
		friend class GUI;
	};
}
