#pragma once

#include "Core.h"

#include "Window.h"

#include "Events/Event.h"
#include "Events/EventManager.h"
#include "Events/WindowEvents.h"

#include "LayerStack.h"

#include "Timestep.h"

#include "Renderer/Renderer.h"

namespace nino
{
	class CORE_API Application
	{
	public:
		Application(const uint32_t& clientWidth, const uint32_t& clientHeight);
		virtual ~Application();

		void OnEvent(Event& event);

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

	private:
		friend int CORE_API CreateApplication(Application* app, const wchar_t* className);
		void Initialize(const wchar_t* className);
		bool OnWindowClose(WindowClosedEvent& event);
		bool OnWindowResize(WindowResizedEvent& event);

	private:
		bool shouldRun = true;
		uint32_t m_Width;
		uint32_t m_Height;
		Log m_Log;
		Window m_Window;
		EventManager m_EventManager;
		LayerStack m_LayerStack;
		Renderer m_Renderer;
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
