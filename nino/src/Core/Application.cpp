#include "corepch.h"
#include "Application.h"

#include "Core/ConfigurationLoader.h"

namespace nino
{
	Application::Application()
	{
		try
		{
			ConfigurationLoader loader(m_Descriptor, "config.ini");

			WindowDescriptor mainWindowDesc = {};
			mainWindowDesc.WindowName = m_Descriptor.ApplicationName;
			mainWindowDesc.Width = m_Descriptor.Width;
			mainWindowDesc.Height = m_Descriptor.Height;
			mainWindowDesc.Maximized = m_Descriptor.Maximized;

			PushWindow(new Window(mainWindowDesc));

			m_EventManager.SetEventCallback(BIND_EVENT(Application::OnEvent));

			m_RenderManager.SetRenderTarget(m_WindowStack.GetWindow(mainWindowDesc.WindowName));
			m_RenderManager.ToggleVSync(m_Descriptor.vSync);

			m_ImGuiLayer = new ImGuiLayer(m_WindowStack.GetWindow(mainWindowDesc.WindowName));
			PushOverlay(m_ImGuiLayer);
		}
		catch(std::exception e)
		{
			MessageBox(nullptr, e.what(), "Nino Exception Caught!", MB_OK | MB_ICONERROR);
			exit(0);
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventManager::Dispatch<WindowClosedEvent>(BIND_EVENT(Application::OnWindowClose));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
		{
			if (!event)
				break;

			(*it)->OnEvent(event);
		}
	}

	void Application::Run()
	{
		for (auto& window : m_WindowStack)
		{
			window.second->Show();
		}

		Timestep timestep;

		while (shouldRun)
		{
			m_EventManager.ProcessEvents();

			timestep.Tick();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
			{
				layer->RenderUserInterface();
			}

			m_ImGuiLayer->End();

			m_RenderManager.EndScenes();
		}

#ifdef CORE_DEBUG
		GraphicsInfo::Set();
		GraphicsInfo::ReportObjects();
		GraphicsInfo::Release();
#endif
	}

	void Application::PushWindow(Window* window)
	{
		WindowDescriptor& descriptor = window->GetDescriptor();
		m_WindowStack.AddWindow(descriptor.WindowName, window);
	}

	void Application::PopWindow(Window* window)
	{
		WindowDescriptor& descriptor = window->GetDescriptor();
		m_WindowStack.RemoveWindow(descriptor.WindowName);
	}

	bool Application::OnWindowClose(WindowClosedEvent& event)
	{
		m_WindowStack.RemoveWindow(event.WindowName);

		if (!m_WindowStack)
		{
			shouldRun = false;
			NINO_CORE_INFO("All windows closed, terminating!");
		}

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

	int CreateApplication(Application* app)
	{
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