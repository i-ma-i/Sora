#pragma once

namespace sora
{
	namespace Config
	{
		void Create(std::filesystem::path yamlpath);

		std::string GetString(std::string_view key);
		bool GetBool(std::string_view key);
		int GetInt(std::string_view key);
		float GetFloat(std::string_view key);
	}
}
