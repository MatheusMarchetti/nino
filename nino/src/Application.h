#pragma once

#include "Core.h"

#include "Window.h"
#include "Events/EventManager.h"
#include "Renderer/Renderer.h"
#include "Events/WindowEvents.h"
#include "LayerStack.h"

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
		Window m_Window;
		EventManager m_EventManager;
		Renderer m_Renderer;
		LayerStack m_LayerStack;
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
