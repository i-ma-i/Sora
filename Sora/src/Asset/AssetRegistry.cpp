#include "AssetRegistry.hpp"

#include "Graphics.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"

#pragma warning(push)
#pragma warning(disable: 4251 4275)
#include <yaml-cpp/yaml.h>
#pragma warning(pop)

namespace
{
	std::unordered_map<std::string, std::string> s_paths;

	void ProcessNode(const YAML::Node& node, const std::string& prefix)
	{
		if (node.IsScalar())
		{
			s_paths[prefix] = node.as<std::string>();
		}
		else if (node.IsMap())
		{
			for (const auto& sub_it : node)
			{
				ProcessNode(sub_it.second, prefix + "." + sub_it.first.as<std::string>());
			}
		}
		else if (node.IsSequence())
		{
			int index = 0;
			for (const auto& seq_it : node)
			{
				ProcessNode(seq_it, prefix + "[" + std::to_string(index++) + "]");
			}
		}
	}
}

namespace sora
{
	AssetRegistry::AssetRegistry(Graphics* graphics)
	{
		s_instance = this;

		// YAMLファイルを読み込む。
		const YAML::Node config = YAML::LoadFile("asset/asset.yaml");

		// データを抽出する。
		for (const auto& it : config)
		{
			ProcessNode(it.second, it.first.as<std::string>());
		}

		m_assetRoot = GetString("Root");

		// アセットを読み込む。
		Register<VertexShader>(
			"Shader.Basic.VS", graphics,
			DirectX::VertexPositionNormalTexture::InputElements,
			DirectX::VertexPositionNormalTexture::InputElementCount
		);
		Register<PixelShader>("Shader.Basic.PS", graphics);
	}

	AssetRegistry::~AssetRegistry()
	{
		s_paths.clear();
		m_registry.clear();

		s_instance = nullptr;
	}

	std::string AssetRegistry::GetString(std::string_view key)
	{
		auto it = s_paths.find(key.data());
		if (it != s_paths.end())
		{
			return it->second;
		}
		__debugbreak();
		return "";
	}
}
