#include "corepch.h"
#include "EventManager.h"

#include "Events/WindowEvents.h"

namespace nino
{
	std::queue<std::shared_ptr<Event>> EventManager::m_EventQueue;
	std::shared_ptr<Event> EventManager::m_CurrentEvent;

	void EventManager::Create()
	{
		m_CurrentEvent = nullptr;
		NINO_CORE_INFO(L"Event subsystem initialized!");
	}

	void EventManager::CollectWindowsEvents()
	{
		MSG m_Msg = {};

		while (PeekMessage(&m_Msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m_Msg);
			DispatchMessage(&m_Msg);
		}
	}

	void EventManager::ProcessEvents()
	{
		while (!m_EventQueue.empty())
		{
			m_CurrentEvent = m_EventQueue.front();
			m_EventQueue.pop();
 
			m_EventCallback(*m_CurrentEvent);
		}
	}

	void EventManager::QueueEvents(const std::shared_ptr<Event>& event)
	{
		m_EventQueue.push(event);
	}

	LRESULT EventManager::EventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SIZE:
		{
			uint32_t width = LOWORD(lParam);
			uint32_t height = HIWORD(lParam);

			std::shared_ptr<WindowResizedEvent> resizedEvent = std::make_shared<WindowResizedEvent>(width, height);
			EventManager::QueueEvents(resizedEvent);

			break;
		}

		case WM_DESTROY:
		{
			std::shared_ptr<WindowClosedEvent> closedEvent = std::make_shared<WindowClosedEvent>();
			EventManager::QueueEvents(closedEvent);

			break;
		}

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		return 0;
	}
}

