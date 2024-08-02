#pragma once

namespace sora
{
	struct DirectionalLight
	{
		DirectionalLight()
			: m_direction(Config::GetVector3("DirectionalLight.m_direction"))
			, m_color(Config::GetVector3("DirectionalLight.m_color"))
			, m_ambient(Config::GetVector3("DirectionalLight.m_ambient"))
		{}

		DirectX::SimpleMath::Vector3 m_direction;
		DirectX::SimpleMath::Vector3 m_color;
		DirectX::SimpleMath::Vector3 m_ambient;
	};

}
