#pragma once

#include "Events/EventManager.h"
#include "Events/WindowEvents.h"
#include "Renderer/Renderer.h"
#include "Core/LayerStack.h"
#include "Core/WindowStack.h"

#include "Debug/GUILayer.h"

namespace nino
{
	struct ApplicationDescriptor
	{
		const char* ApplicationName;
		uint32_t Width;
		uint32_t Height;
	};

	class Application
	{
	private:
		friend int CreateApplication(Application* app);

	public:
		Application(const ApplicationDescriptor& descriptor);
		virtual ~Application();

		void OnEvent(Event& event);

		void Run();

		void PushWindow(Window& window);
		void PopWindow(Window& window);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

	private:
		bool OnWindowClose(WindowClosedEvent& event);

	private:
		ApplicationDescriptor m_Descriptor;
		bool shouldRun = true;
		EventManager m_EventManager;
		Renderer m_Renderer;
		WindowStack m_WindowStack;
		LayerStack m_LayerStack;
		GUILayer* m_ImGuiLayer;
	};
}

#ifdef CORE_RELEASE
#define InitializeEngine(appclass, descriptor) \
	int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) \
	{ \
		nino::Log::Init(); \
		return nino::CreateApplication(new appclass(descriptor)); \
	}
#else
#define InitializeEngine(appclass, descriptor) \
	int main() \
	{ \
		nino::Log::Init(); \
		return nino::CreateApplication(new appclass(descriptor)); \
	}
#endif
