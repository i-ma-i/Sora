#pragma once

namespace sora
{
	class Camera
	{
	public:
		struct Config
		{
			DirectX::SimpleMath::Vector3 position = { 0.0f, 5.0f, -10.0f };
			float yawRad = 0.0f;
			float pitchRad = -0.5f;
			float moveSpeed = 15.0f;
			float rotateSpeedRad = 0.05f;
			float zoomSpeed = 20.0f;
			float fov = DirectX::XM_PIDIV4;
			float aspectRate = 16.0f / 9.0f;
			float nearZ = 1.0f;
			float farZ = 1000.0f;
		};

	public:
		Camera(const Config& config = Config())
		: m_position(config.position)
		, m_yawRad(config.yawRad)
		, m_pitchRad(config.pitchRad)
		, m_moveSpeed(config.moveSpeed)
		, m_rotateSpeedRad(config.rotateSpeedRad)
		, m_zoomSpeed(config.zoomSpeed)
		{
			CreatePerspective(config.fov, config.aspectRate, config.nearZ, config.farZ);
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
