#include "AssetRegistry.hpp"

#include "Graphics.hpp"
#include "Shader.hpp"

namespace sora
{
	AssetRegistry::AssetRegistry(Graphics* graphics)
	{
		s_instance = this;

		m_assetRoot = Config::GetString("Directory.asset");

		// アセットを登録する。
		Register<VertexShader>(
			"Shader.Basic.VS", graphics,
			DirectX::VertexPositionNormalTexture::InputElements,
			DirectX::VertexPositionNormalTexture::InputElementCount
		);
		Register<PixelShader>("Shader.Basic.PS", graphics);
	}

	AssetRegistry::~AssetRegistry()
	{
		m_registry.clear();

		s_instance = nullptr;
	}
}
