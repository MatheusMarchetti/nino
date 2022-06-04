#pragma once

#include "Core/Window.h"
#include "Events/EventManager.h"
#include "Renderer/Renderer.h"
#include "Events/WindowEvents.h"
#include "Core/LayerStack.h"

#include "Debug/GUILayer.h"

namespace nino
{
	class Application
	{
	private:
		friend int CreateApplication(Application* app, const char* name);

	public:
		Application(const uint32_t& clientWidth, const uint32_t& clientHeight);
		virtual ~Application();

		void SetWindowTitle(const char* name);

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
		GUILayer* m_ImGuiLayer;
	};
}

#ifdef CORE_RELEASE
#define InitializeEngine(appclass) \
	int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) \
	{ \
		nino::Log::Init(); \
		return nino::CreateApplication(new appclass(), #appclass); \
	}
#else
#define InitializeEngine(appclass) \
	int main() \
	{ \
		nino::Log::Init(); \
		return nino::CreateApplication(new appclass(), #appclass); \
	}
#endif
