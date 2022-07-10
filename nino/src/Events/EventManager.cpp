#include "corepch.h"
#include "EventManager.h"

#include "Events/ApplicationEvents.h"

namespace nino
{
	static constexpr size_t max_events = 10;
	static size_t s_Head;
	static size_t s_Tail;
	static char s_WindowName[1000] = "";
	Event* EventManager::s_currentEvent;
	std::vector<Event*> EventManager::s_EventQueue;

	EventManager::EventManager()
	{
		s_Head = 0;
		s_Tail = 0;

		s_EventQueue.resize(max_events);
	}

	void EventManager::QueueEvent(Event* event)
	{
		s_EventQueue[s_Tail] = event;

		s_Tail = (s_Tail + 1) % max_events;
	}

	void EventManager::ProcessEvents()
	{
		while (s_Head != s_Tail)
		{
			s_currentEvent = s_EventQueue[s_Head];
			s_EventQueue[s_Head] = nullptr;
			DispatchEvent(s_currentEvent);

			s_Head = (s_Head + 1) % max_events;
		}

		MSG msg = {};
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void EventManager::DispatchEvent(Event* event)
	{
		m_EventCallback(*event);
	}

	EventManager::~EventManager()
	{
		for (auto& event : s_EventQueue)
		{
			delete event;
		}
	}

	LRESULT EventManager::EventHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
		{
			ThrowOnError(GetClassName(hWnd, s_WindowName, 1000));

			QueueEvent(new WindowClosedEvent(s_WindowName));
		}
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}
}