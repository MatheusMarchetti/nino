#pragma once

#include "Core/Core.h"

#include "Events/Event.h"

namespace nino
{
	class CORE_API EventManager
	{
	public:
		EventManager();
		~EventManager() {}
		EventManager(EventManager const&) = delete;
		void operator=(EventManager const&) = delete;
		
		void CollectWindowsEvents();
		void SetEventCallback(const std::function<void(Event&)>& callback) { m_EventCallback = callback; }
		void ProcessEvents();

		static void QueueEvents(const std::shared_ptr<Event>& event);
		
		template<typename T>
		static void Dispatch(std::function<bool(T&)> func)
		{
			if (typeid(T).hash_code() == m_CurrentEvent->GetEventID())
			{
				m_CurrentEvent->Handled = func(*(T*)&(*m_CurrentEvent));
			}
		}
		
		LRESULT CALLBACK EventHandler(HWND, UINT, WPARAM, LPARAM);

	private:
		static std::queue<std::shared_ptr<Event>> m_EventQueue;
		std::function<void(Event&)> m_EventCallback;
		static std::shared_ptr<Event> m_CurrentEvent;
	};
}