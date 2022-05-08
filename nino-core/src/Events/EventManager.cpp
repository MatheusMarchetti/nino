#include "corepch.h"
#include "EventManager.h"

namespace nino
{
	std::queue<std::shared_ptr<Event>> EventManager::m_EventQueue;
	std::shared_ptr<Event> EventManager::m_CurrentEvent;

	void EventManager::Create()
	{
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
			NINO_CORE_WARN(L"Current event: {}", m_CurrentEvent->ToString());
 
			m_EventCallback(*m_CurrentEvent);


		}
	}

	void EventManager::QueueEvents(const std::shared_ptr<Event>& event)
	{
		NINO_CORE_WARN(L"Event received on queue: {}", event->ToString());
		m_EventQueue.push(event);
		NINO_CORE_WARN(L"First event on queue: {}", m_EventQueue.front()->ToString());
		NINO_CORE_WARN(L"Number of events on queue: {}", m_EventQueue.size());
	}

	LRESULT EventManager::EventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SIZE:
		{

			break;
		}

		case WM_DESTROY:
		{
			std::shared_ptr<WindowClosedEvent> closedEvent = std::make_shared<WindowClosedEvent>();
			NINO_CORE_WARN(L"Created event: {}", closedEvent->ToString());
			EventManager::QueueEvents(closedEvent);

			break;
		}

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		return 0;
	}
}

