#include "corepch.h"
#include "Application.h"

namespace nino
{
	Application::Application(const uint32_t& clientWidth, const uint32_t& clientHeight)
		: m_Width(std::max(1u, clientWidth)), m_Height(std::max(1u, clientHeight))
	{
		m_EventManager.SetEventCallback(BIND_EVENT(Application::OnEvent));
	}

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
		m_Renderer.Create(m_Window.GetWindow(), m_Width, m_Height);
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
			m_EventManager.CollectWindowsEvents();
			m_EventManager.ProcessEvents();

			Timestep timestep;
			timestep.Tick();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(timestep);
			}
		}
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

		m_Renderer.Resize(m_Width, m_Height);

		return true;
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
		app->Initialize(className);
		
		app->Run();

		delete app;

		return 0;
	}
}