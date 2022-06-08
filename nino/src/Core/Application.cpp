#include "corepch.h"
#include "Application.h"

#include "imgui.h"

namespace nino
{
	Application::Application(const uint32_t& clientWidth, const uint32_t& clientHeight)
	{
		try
		{
			m_Window.Init("nino Game Application", std::max(1u, clientWidth), std::max(1u, clientHeight));
			m_Renderer.Init(&m_Window);

			m_EventManager.SetEventCallback(BIND_EVENT(Application::OnEvent));

			m_ImGuiLayer = new GUILayer(&m_Window);
			PushOverlay(m_ImGuiLayer);
		}
		catch (std::exception e)
		{
			MessageBox(nullptr, e.what(), "Nino Exception Caught!", MB_OK | MB_ICONERROR);
			exit(0);
		}
	}

	Application::~Application()
	{
		
	}

	void Application::SetWindowTitle(const char* name)
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

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
			{
				layer->RenderUI();
			}

			m_ImGuiLayer->End();

			nino::Renderer::EndScenes();
		}

#ifdef CORE_DEBUG
		GraphicsInfo::Set();
		GraphicsInfo::ReportObjects();
		GraphicsInfo::Release();
#endif
	}

	bool Application::OnWindowClose(WindowClosedEvent& event)
	{
		shouldRun = false;

		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.AttachLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.AttachOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.DetachLayer(layer);
	}

	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack.DetachOverlay(overlay);
	}

	int CreateApplication(Application* app, const char* name)
	{
		app->SetWindowTitle(name);

		try
		{
			app->Run();
		}
		catch (std::exception e)
		{
			MessageBox(nullptr, e.what(), "Nino Exception Caught!", MB_OK | MB_ICONERROR);
			exit(0);
		}

		delete app;

		return 0;
	}
}