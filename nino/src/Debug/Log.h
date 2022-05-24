#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <comdef.h>

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

	template<typename T>
	T ThrowOnError(T returnValue, int errorLine, const wchar_t* file)
	{
		DWORD error;
		HRESULT hresultError = S_OK;

		if (typeid(T) != typeid(HRESULT))
		{
			if (returnValue == 0)
			{
				error = GetLastError();
				hresultError = HRESULT_FROM_WIN32(error);
			}
		}
		else
		{
			if (FAILED(returnValue))
			{
				hresultError = (HRESULT)returnValue;
			}
		}

		if (FAILED(hresultError))
		{
			_com_error err(hresultError);
			error = HRESULT_CODE(hresultError);

			NINO_CORE_ERROR(L"[ERROR] {:#4x}: {} ({}, {})", error, err.ErrorMessage(), file, errorLine);
		}

		return returnValue;
	}

#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define ThrowOnError(x) ThrowOnError(x, __LINE__, __WFILE__);
}