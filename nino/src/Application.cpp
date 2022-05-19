#include "corepch.h"
#include "Application.h"

namespace nino
{
	Application::Application(const uint32_t& clientWidth, const uint32_t& clientHeight)
		: m_Window(L"nino Game Application", std::max(1u, clientWidth), std::max(1u, clientHeight)), m_Renderer(&m_Window)
	{
		m_EventManager.SetEventCallback(BIND_EVENT(Application::OnEvent));
	}

	Application::~Application()
	{}

	void Application::SetWindowTitle(const wchar_t* name)
	{
		SetWindowText(m_Window.GetWindow(), name);
	}

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
		Renderer::ToggleVSync(true);

		float color[] = { 0.2f, 0.2f, 0.2f, 1.0f };

		while (shouldRun)
		{
			Renderer::Clear(color);

			m_EventManager.CollectWindowsEvents();
			m_EventManager.ProcessEvents();

			Timestep timestep;
			timestep.Tick();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(timestep);
			}

			Renderer::Draw();
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

	int CreateApplication(Application* app, const wchar_t* name)
	{
		app->SetWindowTitle(name);

		app->Run();

		delete app;

		return 0;
	}
}