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
		static void Dispatch(std::function<bool(T&)> func, Event* event)
		{
			if (event)
			{
				if (typeid(T).hash_code() == event->GetEventID())
				{
					if (!func(*(T*)&(*event)))
					{
						event->Handled = false;
					}
					else
					{
						event->Handled = true;
						delete event;
						event = nullptr;
					}
				}
			}
			else
			{
				delete event;
				event = nullptr;
			}
		}

		static LRESULT CALLBACK EventHandler(HWND, UINT, WPARAM, LPARAM);

	private:
		void DispatchEvent(Event* event);

	private:
		std::function<void(Event&)> m_EventCallback;
		inline static std::vector<Event*> s_EventQueue;
		inline static EventManager* s_Instance;
	};
}