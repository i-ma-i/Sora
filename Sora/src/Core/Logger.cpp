#include "Logger.hpp"

#include <iostream>
#include <magic_enum.hpp>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sora::Logger
{
	void Create()
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
			std::cerr << "Failed to setup logger. [exception:" << ex.what() << "]" << std::endl;
			assert(false);
		}
	}

	void Cleanup()
	{
		LOG_INFO(__func__);
		spdlog::default_logger()->flush();
		spdlog::drop_all();
	}
}
