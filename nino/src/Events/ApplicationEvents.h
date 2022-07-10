#pragma once

#include "Core/Core.h"
#include "Core/Window.h"
#include "Events/Event.h"

namespace nino
{
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent(const char* windowName)
			: WindowName(windowName)
		{
			m_EventID = EVENT_ID(WindowClosedEvent);
		}

	public:
		const char* WindowName;
	};
}