#pragma once

namespace sora
{
	struct CBLight
	{
		DirectX::SimpleMath::Vector4 Direction;
		DirectX::SimpleMath::Vector4 CameraPosition;
	};

	class DirectionalLight
	{
	public:
		DirectionalLight()
			: m_direction(Config::GetVector3("DirectionalLight.m_direction"))
		{}

		DirectX::SimpleMath::Vector4 GetDirection() const
		{
			return DirectX::SimpleMath::Vector4{ m_direction.x, m_direction.y, m_direction.z, 0.0f };
		};

	private:
		DirectX::SimpleMath::Vector3 m_direction;

	private:
		friend class GUI;
	};

}
