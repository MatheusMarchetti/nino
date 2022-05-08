#include "corepch.h"
#include "Application.h"

#include "Events/WindowEvents.h"

namespace nino
{
	Application::Application(const uint32_t& clientWidth, const uint32_t& clientHeight)
		: m_Width(clientWidth), m_Height(clientHeight)
	{}

	Application::~Application()
	{}

	void Application::OnEvent(Event& event)
	{
		NINO_CORE_WARN(L"Received event on Application: {}", event.ToString());
		EventManager::Dispatch<WindowClosedEvent>(BIND_EVENT(Application::OnWindowClose));
	}

	void Application::Initialize(const wchar_t* className)
	{
		m_Window.Create(className, m_Width, m_Height);
		m_EventManager.Create();
		m_EventManager.SetEventCallback(BIND_EVENT(Application::OnEvent));
	}

	bool Application::OnWindowClose(Event& event)
	{
		NINO_CORE_WARN(L"Received event on OnWindowClose: {}", event.ToString());
		shouldRun = false;

		NINO_CORE_WARN(L"Event handled!");

		return true;
	}

	void Application::Run()
	{
		m_Window.Show();

		while (shouldRun)
		{
			m_EventManager.CollectWindowsEvents();
			m_EventManager.ProcessEvents();
		}
	}

	int CreateApplication(Application* app, const wchar_t* className)
	{
		Log::Init();

		NINO_CORE_INFO("Initializing subsystems...");

		app->Initialize(className);

		NINO_CORE_INFO("All subsystems initialized");

		NINO_INFO(L"{0} started", className);
		
		app->Run();

		NINO_INFO(L"Closing {}", className);

		delete app;

		return 0;
	}
}