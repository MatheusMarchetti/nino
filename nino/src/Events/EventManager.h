#pragma once

#include "Events/Event.h"

namespace nino
{
	class EventManager
	{
	public:
		EventManager();
		~EventManager() = default;

		void SetEventCallback(const std::function<void(Event&)>& callback) { m_EventCallback = callback; }

		static void QueueEvent(Event* event);
		void ProcessEvents();
		
		template<typename T>
		static void Dispatch(std::function<bool(T&)> func)
		{
			if (s_currentEvent)
			{
				if (typeid(T).hash_code() == s_currentEvent->GetEventID())
				{
					if (!func(*(T*)&(*s_currentEvent)))
					{
						s_currentEvent->Handled = false;
					}
					else
					{
						s_currentEvent->Handled = true;
						delete s_currentEvent;
						s_currentEvent = nullptr;
					}
				}
			}
			else
			{
				delete s_currentEvent;
				s_currentEvent = nullptr;
			}
		}

		static LRESULT CALLBACK EventHandler(HWND, UINT, WPARAM, LPARAM);

	private:
		void DispatchEvent(Event* event);

	private:
		std::function<void(Event&)> m_EventCallback;
		inline static Event* s_currentEvent;
		inline static std::vector<Event*> s_EventQueue;
	};
}