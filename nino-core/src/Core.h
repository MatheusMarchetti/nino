#pragma once

#ifdef CORE_BUILD_DLL
	#define CORE_API __declspec(dllexport)
#else
	#define CORE_API __declspec(dllimport)
#endif

#define BIND_EVENT(fn) std::bind(&fn, this, std::placeholders::_1)

#define EVENT_ID(event) typeid(event).hash_code()