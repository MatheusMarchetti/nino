#pragma once

#include "Core.h"

#include "Events/WindowEvents.h"
#include "LayerStack.h"

namespace nino
{
	class Window;
	class EventManager;
	class Renderer;
}

namespace nino
{
	class CORE_API Application
	{
	private:
		friend int CORE_API CreateApplication(Application* app, const wchar_t* name);

	public:
		Application(const uint32_t& clientWidth, const uint32_t& clientHeight);
		virtual ~Application();

		void SetWindowTitle(const wchar_t* name);

		void OnEvent(Event& event);

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

	private:
		bool OnWindowClose(WindowClosedEvent& event);

	private:
		bool shouldRun = true;
		LayerStack m_LayerStack;
		std::shared_ptr<Window> m_Window;
		std::shared_ptr<EventManager> m_EventManager;
		std::shared_ptr<Renderer> m_Renderer;
	};
}

#ifdef CORE_RELEASE
#define InitializeEngine(appclass) \
	int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) \
	{ \
		nino::Log::Init(); \
		return nino::CreateApplication(new appclass()); \
	}
#else
#define InitializeEngine(appclass) \
	int main() \
	{ \
		nino::Log::Init(); \
		return nino::CreateApplication(new appclass(), L#appclass); \
	}
#endif
