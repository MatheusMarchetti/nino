#pragma once

#include <dxgidebug.h>

namespace nino
{
	class GraphicsInfo
	{
	public:
		static void Set();
		static std::vector<std::string> GetMessages();
		static std::string ThrowException(std::vector<std::string>& errorMessages);
		static void Release();

	private:
		static std::string m_ErrorMessage;
		static unsigned long long m_NextMessage;
		static IDXGIInfoQueue* m_InfoQueue;
	};

#ifdef CORE_DEBUG
#define ThrowInfoMessage(action) \
	GraphicsInfo::Set(); \
	(action); \
	auto error = GraphicsInfo::GetMessages(); \
	if(!error.empty()) \
	throw std::exception(GraphicsInfo::ThrowException(error).c_str());
#else
#define ThrowInfoMessage(action) (action)
#endif
}