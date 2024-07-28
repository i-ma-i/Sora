#pragma once

namespace sora
{
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

	namespace Logger
	{
		/// @brief ロガーを作成する。
		static void Create()
		{
			try
			{
				std::filesystem::path filePath = std::filesystem::current_path() / Config::GetString("logger.filePath");
				const std::string pathString = filePath.make_preferred().string();
				const std::string filePattern = Config::GetString("logger.filePattern");
				const std::string consolePattern = Config::GetString("logger.consolePattern");
				const spdlog::level::level_enum level = magic_enum::enum_cast<spdlog::level::level_enum>(Config::GetString("logger.level")).value();


				std::vector<spdlog::sink_ptr> sinks;
				sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(pathString, true));
				sinks[0]->set_pattern(filePattern.c_str());

#if _CONSOLE
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				sinks[1]->set_pattern(consolePattern.c_str());
#endif
				auto logger = std::make_shared<spdlog::logger>("Logger", sinks.begin(), sinks.end());


				spdlog::set_default_logger(logger);
				spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
				// エラー以上のログは即時書き込むようにする。
				spdlog::flush_on(spdlog::level::err);

				LOG_INFO("Successfully to setup. LogLevel[{}] LogFile[{}]", (int)level, pathString);
			}
			catch (const spdlog::spdlog_ex& ex)
			{
				printf("Failed to setup logger. [exception:%s]\n", ex.what());
				DebugBreak();
			}
		}

		/// @brief ロガーのクリーンアップを行う。
		static void Cleanup()
		{
			LOG_INFO(__func__);
			spdlog::default_logger()->flush();
			spdlog::drop_all();
		}
	};


}
