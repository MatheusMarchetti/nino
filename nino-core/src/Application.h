#pragma once

#include "Core.h"

#include "Window.h"
#include "Events/EventManager.h"
#include "Events/Event.h"

namespace nino
{
	class CORE_API Application
	{
	public:
		Application(const uint32_t& clientWidth, const uint32_t& clientHeight);
		virtual ~Application();

		void OnEvent(Event& event);

		void Run();

	private:
		friend int CORE_API CreateApplication(Application* app, const wchar_t* className);
		void Initialize(const wchar_t* className);
		bool OnWindowClose(Event& event);

	private:
		bool shouldRun = true;
		uint32_t m_Width;
		uint32_t m_Height;
		Window m_Window;
		EventManager m_EventManager;
	};

}

#ifdef CORE_RELEASE
#define InitializeEngine(appclass) \
	int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int) \
	{ \
		return nino::CreateApplication(new appclass(), L#appclass); \
	}
#else
#define InitializeEngine(appclass) \
	int main() \
	{ \
		return nino::CreateApplication(new appclass(), L#appclass); \
	}
#endif
