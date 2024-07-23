#pragma once

namespace sora
{
	/// @brief 環境変数クラス。
	class Env
	{
	public:
		/// @brief 環境変数を作成する。
		/// @param yamlpath YAMLファイルのパス。
		/// @details YAMLファイルを読み込み、環境変数を作成する。
		/// @details 既に作成済みの場合は、上書きされる。
		static void Create(std::filesystem::path yamlpath);

		/// @brief 文字列を取得する。
		/// @param key キー。
		/// @return 文字列。
		/// @details キーに対応する文字列を取得する。
		static std::string GetString(std::string_view key);

		/// @brief 整数を取得する。
		/// @param key キー。
		/// @return 整数。
		/// @details キーに対応する整数を取得する。
		static int GetInt(std::string_view key);

		/// @brief 浮動小数点数を取得する。
		/// @param key キー。
		/// @return 浮動小数点数。
		/// @details キーに対応する浮動小数点数を取得する。
		static float GetFloat(std::string_view key);
	};
}
