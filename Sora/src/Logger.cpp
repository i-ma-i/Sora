#include "Logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>

namespace sora::Logger
{
	void Setup(const Config& config)
	{
		try
		{
			std::vector<spdlog::sink_ptr> sinks;

			auto copy_filepath = config.filepath;
			const std::string pathstring = copy_filepath.make_preferred().string();
			sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(pathstring, true));
			sinks[0]->set_pattern(config.filePattern.c_str());

#if _CONSOLE
			sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
			sinks[1]->set_pattern(config.consolePattern.c_str());
#endif
			auto logger = std::make_shared<spdlog::logger>("Logger", sinks.begin(), sinks.end());


			spdlog::set_default_logger(logger);
			spdlog::set_level(static_cast<spdlog::level::level_enum>(config.level));
			// エラー以上のログは即時書き込むようにする。
			spdlog::flush_on(spdlog::level::err);

			LOG_INFO("Successfully to setup. LogLevel[{}] LogFile[{}]", (int)config.level, pathstring);
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
