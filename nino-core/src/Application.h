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
	private:
		friend int CORE_API CreateApplication(Application* app);

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
		bool OnWindowClose(WindowClosedEvent& event);

	private:
		bool shouldRun = true;
		Log m_Log;
		Window m_Window;
		EventManager m_EventManager;
		LayerStack m_LayerStack;
		Renderer m_Renderer;
	};

}

#ifdef CORE_RELEASE
#define InitializeEngine(appclass) \
	int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) \
	{ \
		return nino::CreateApplication(new appclass()); \
	}
#else
#define InitializeEngine(appclass) \
	int main() \
	{ \
		return nino::CreateApplication(new appclass()); \
	}
#endif
