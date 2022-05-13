#include "corepch.h"
#include "Application.h"

namespace nino
{
	Application::Application(const uint32_t& clientWidth, const uint32_t& clientHeight)
	{
		m_Window.Create(L"nino Game Application", std::max(1u, clientWidth), std::max(1u, clientHeight));
		m_Renderer.Create(m_Window.GetWindow(), std::max(1u, clientWidth), std::max(1u, clientHeight));
		m_Renderer.ToggleVSync(true);
		m_EventManager.SetEventCallback(BIND_EVENT(Application::OnEvent));
	}

	Application::~Application()
	{}

	void Application::OnEvent(Event& event)
	{
		EventManager::Dispatch<WindowClosedEvent>(BIND_EVENT(Application::OnWindowClose));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
		{
			if (event.Handled)
				break;

			(*it)->OnEvent(event);
		}
	}

	void Application::Run()
	{
		m_Window.Show();

		while (shouldRun)
		{
			m_EventManager.CollectWindowsEvents();
			m_EventManager.ProcessEvents();

			Timestep timestep;
			timestep.Tick();

			//Renderer test
			m_Renderer.Clear(0.2f, 0.8f, 0.5f, 1.0f);
			m_Renderer.SubmitVertices();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(timestep);
			}

			m_Renderer.Render();
		}
	}

	bool Application::OnWindowClose(WindowClosedEvent& event)
	{
		shouldRun = false;

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

	int CreateApplication(Application* app)
	{
		app->Run();

		delete app;

		return 0;
	}
}