#include "Camera.hpp"
#include "MouseInput.hpp"
#include "Input.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"

using namespace DirectX;
using namespace DirectX::SimpleMath;

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
	DirectX::SimpleMath::Matrix Camera::GetView() const
	{
		return XMMatrixLookToLH(m_position, GetForward(), Vector3::Up);
	}

	DirectX::SimpleMath::Matrix Camera::CreatePerspective(float fov, float aspectRate, float nearZ, float farZ)
	{
		XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(fov, aspectRate, nearZ, farZ);
		m_projection = perspectiveMatrix;
		return m_projection;
	}

	DirectX::SimpleMath::Matrix Camera::CreateOrthographic(float width, float height, float nearZ, float farZ)
	{
		XMMATRIX orthographicMatrix = XMMatrixOrthographicLH(width, height, nearZ, farZ);
		m_projection = orthographicMatrix;
		return m_projection;
	}

	void Camera::Update(float deltaTime)
	{
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

	DirectX::SimpleMath::Vector3 Camera::GetForward() const
	{
		return {
			cosf(m_pitchRad) * sinf(m_yawRad),
			sinf(m_pitchRad),
			cosf(m_pitchRad) * cosf(m_yawRad)
		};
	}
}
