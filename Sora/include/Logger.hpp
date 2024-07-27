﻿#pragma once
#include <spdlog/spdlog.h>

namespace sora
{
	namespace Logger
	{
		/// @brief ロガーの設定を行うための構造体。
		struct Config
		{
			std::filesystem::path filepath;
			spdlog::level::level_enum level = spdlog::level::off;
			std::string filePattern;
			std::string consolePattern;
		};

		/// @brief ロガーの設定を行う。
		/// @param config ロガーの設定。
		void Setup(const Config& config);

		/// @brief ロガーのクリーンアップを行う。
		void Cleanup();
	};

#define LOG_TRACE(...)		spdlog::log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...)		spdlog::log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...)		spdlog::log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::info, __VA_ARGS__)
#define LOG_WARNING(...)	spdlog::log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)		spdlog::log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, __VA_ARGS__)
#define LOG_CRITICAL(...)	spdlog::log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__)
#if DEBUG || _DEBUG
#define LOG_TRACE_FUNC()	spdlog::log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::trace, "#"+std::string(__func__)+"()")
#else
#define LOG_TRACE_FUNC() (void)0;
#endif
}