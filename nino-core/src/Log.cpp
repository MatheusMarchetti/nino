#include "corepch.h"

namespace nino
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::vector<spdlog::sink_ptr> Log::sinks;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(L"logs/Log.log", true));
		
		s_CoreLogger = std::make_shared<spdlog::logger>("NINO-CORE", sinks.begin(), sinks.end());
		s_ClientLogger = std::make_shared<spdlog::logger>("APP", sinks.begin(), sinks.end());

		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}

