#pragma once

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
			CreatePerspective(
				DirectX::XM_PIDIV4,
				16.0f / 9.0f,
				Config::GetFloat("camera.nearZ"),
				Config::GetFloat("camera.farZ")
			);
		}

		DirectX::SimpleMath::Matrix GetView() const;
		DirectX::SimpleMath::Matrix GetProjection() const { return m_projection; }
		DirectX::SimpleMath::Matrix GetViewProjection() const { return GetView() * m_projection;}
		DirectX::SimpleMath::Matrix CreatePerspective(float fov, float aspectRate, float nearZ, float farZ);
		DirectX::SimpleMath::Matrix CreateOrthographic(float width, float height, float nearZ, float farZ);

		void Update(float deltaTime);

	private:
		DirectX::SimpleMath::Vector3 GetForward() const;

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
