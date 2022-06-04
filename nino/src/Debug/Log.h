#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace nino
{
	class Log
	{
	public:
		Log() { Init(); }
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::vector<spdlog::sink_ptr> sinks;
	};

#define NINO_CORE_ERROR(...) nino::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NINO_CORE_TRACE(...) nino::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NINO_CORE_INFO(...) nino::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NINO_CORE_WARN(...) nino::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NINO_CORE_FATAL(...) nino::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define NINO_ERROR(...) nino::Log::GetClientLogger()->error(__VA_ARGS__)
#define NINO_TRACE(...) nino::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NINO_INFO(...) nino::Log::GetClientLogger()->info(__VA_ARGS__)
#define NINO_WARN(...) nino::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NINO_FATAL(...) nino::Log::GetClientLogger()->fatal(__VA_ARGS__)

}