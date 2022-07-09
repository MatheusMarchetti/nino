#pragma once

#include <comdef.h>

namespace nino
{
	class WindowInfo
	{
	public:
		template<typename T>
		static T ThrowOnError(T returnValue, int errorLine, const char* file);

	private:
		static std::string m_ErrorMessage;
	};

	template<typename T>
	T WindowInfo::ThrowOnError(T returnValue, int errorLine, const char* file)
	{
#ifdef CORE_DEBUG
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

			m_ErrorMessage = std::to_string(error);
			m_ErrorMessage += ": ";
			m_ErrorMessage += err.ErrorMessage();
			m_ErrorMessage += " (";
			m_ErrorMessage += file;
			m_ErrorMessage += ", ";
			m_ErrorMessage += std::to_string(errorLine);
			m_ErrorMessage += ")";

			throw std::exception(m_ErrorMessage.c_str());
		}
#endif
		return returnValue;
	}

#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define ThrowOnError(x) WindowInfo::ThrowOnError(x, __LINE__, __FILE__);
}
