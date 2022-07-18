#pragma once

#include "Core/LayerStack.h"
#include "Core/WindowStack.h"

#include "Events/ApplicationEvents.h"
#include "Events/EventManager.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"

#include "Renderer/RenderManager.h"

#include "Debug/ImGuiLayer.h"

namespace nino
{
	struct ApplicationDescriptor
	{
		std::string ApplicationName;
		uint32_t Width;
		uint32_t Height;
		bool Maximized = false;
		bool vSync = true;
	};

	class Application
	{
	private:
		friend int CreateApplication(Application* app);

	public:
		Application();
		virtual ~Application() = default;

		Window* GetNativeWindow() { return m_WindowStack.GetWindow(m_Descriptor.ApplicationName); }

		void OnEvent(Event& event);

		void PushWindow(Window* window);
		void PopWindow(Window* window);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

	private:
		void Run();
		void Update();

	private:
		bool OnWindowClose(WindowClosedEvent& event);
		bool OnWindowResize(WindowResizedEvent& event);

	private:
		bool m_ShouldRun = true;
		bool m_ShouldRender = true;
		ApplicationDescriptor m_Descriptor;
		EventManager m_EventManager;
		RenderManager m_RenderManager;
		WindowStack m_WindowStack;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		Timestep m_Timestep;
	};
}

#ifdef CORE_RELEASE
#define InitializeEngine(appclass)									\
	int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)			\
	{																\
		nino::Log::Init();											\
		return nino::CreateApplication(new appclass(descriptor));	\
	}
#else
#define InitializeEngine(appclass)									\
	int main() { \
		nino::Log::Init(); \
		return nino::CreateApplication(new appclass()); \
	}
#endif
