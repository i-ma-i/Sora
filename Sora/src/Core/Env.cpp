#include "Env.hpp"

#pragma warning(push)
#pragma warning(disable: 4251 4275)
#include <yaml-cpp/yaml.h>
#pragma warning(pop)

namespace
{
	std::unordered_map<std::string, std::string> s_env;

	void ProcessNode(const YAML::Node& node, const std::string& prefix)
	{
		if (node.IsScalar())
		{
			s_env[prefix] = node.as<std::string>();
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
				ProcessNode(seq_it, prefix + "[" + std::to_string(index) + "]");
				++index;
			}
		}
	}
}

namespace sora
{
	void Env::Create(std::filesystem::path filepath)
	{
		// 最初にクリアしておく。
		s_env.clear();

		// YAMLファイルを読み込む。
		const YAML::Node config = YAML::LoadFile(filepath.make_preferred().string());

		// データを抽出する。
		for (const auto& it : config)
		{
			ProcessNode(it.second, it.first.as<std::string>());
		}
	}

	std::string Env::GetString(std::string_view key)
	{
		auto it = s_env.find(key.data());
		if (it != s_env.end())
		{
			return it->second;
		}
		__debugbreak();
	}

	int Env::GetInt(std::string_view key)
	{
		auto it = s_env.find(key.data());
		if (it != s_env.end())
		{
			return std::stoi(it->second);
		}
		__debugbreak();
	}

	float Env::GetFloat(std::string_view key)
	{
		auto it = s_env.find(key.data());
		if (it != s_env.end())
		{
			return std::stof(it->second);
		}
		__debugbreak();
	}
}
