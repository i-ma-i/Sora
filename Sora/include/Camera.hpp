#pragma once

namespace sora
{
	class Camera
	{
	public:
		Camera(
			DirectX::SimpleMath::Vector3 position = { 0.0f, 5.0f, -10.0f },
			float yaw = 0.0f,
			float pitch = 0.0f,
			float moveSpeed = 10.0f,
			float rotateSpeed = 2.0f,
			float zoomSpeed = 10.0f
		)
		: m_position(position)
		, m_yaw(yaw)
		, m_pitch(pitch)
		, m_moveSpeed(moveSpeed)
		, m_rotateSpeed(rotateSpeed)
		, m_zoomSpeed(zoomSpeed)
		{}

		DirectX::SimpleMath::Matrix GetViewMatrix() const;

		void Update(float deltaTime);
		float GetYaw() const { return m_yaw; }
		float GetPitch() const { return m_pitch; }
	private:
		DirectX::SimpleMath::Vector3 GetForward() const;

	public:
		float m_moveSpeed, m_rotateSpeed, m_zoomSpeed;

	private:
		DirectX::SimpleMath::Vector3 m_position;
		float m_yaw, m_pitch;
	};
}
