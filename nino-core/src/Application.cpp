#include "corepch.h"
#include "Application.h"

#include "LayerStack.h"

namespace nino
{
	Application::Application(const uint32_t& clientWidth, const uint32_t& clientHeight)
		: m_Width(clientWidth), m_Height(clientHeight)
	{}

	Application::~Application()
	{}

	void Application::OnEvent(Event& event)
	{
		EventManager::Dispatch<WindowClosedEvent>(BIND_EVENT(Application::OnWindowClose));
		EventManager::Dispatch<WindowResizedEvent>(BIND_EVENT(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
		{
			if (event.Handled)
				break;

			(*it)->OnEvent(event);
		}
	}

	void Application::Initialize(const wchar_t* className)
	{
		m_Window.Create(className, m_Width, m_Height);
		m_EventManager.Create();
		m_EventManager.SetEventCallback(BIND_EVENT(Application::OnEvent));
	}

	bool Application::OnWindowClose(WindowClosedEvent& event)
	{
		shouldRun = false;

		return true;
	}

	bool Application::OnWindowResize(WindowResizedEvent& event)
	{
		m_Width = event.GetWidth();
		m_Height = event.GetHeight();

		m_Window.Resize(m_Width, m_Height);

		return true;
	}

	void Application::Run()
	{
		m_Window.Show();

		uint64_t countsPerSecond;
		uint64_t currentTime;
		uint64_t previousTime = 0;
		double secondsPerCount;

		while (shouldRun)
		{
			QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
			secondsPerCount = 1.0 / (double)countsPerSecond;

			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

			Timestep timestep = (currentTime - previousTime) * secondsPerCount;

			previousTime = currentTime;

			m_EventManager.CollectWindowsEvents();
			m_EventManager.ProcessEvents();

			if (timestep > 1.0f) timestep = 0.0f;

			NINO_CORE_TRACE(L"Timestep: {}", timestep);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(timestep);
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.AttachLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.AttachOverlay(overlay);
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.DetachLayer(layer);
	}

	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack.DetachOverlay(overlay);
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