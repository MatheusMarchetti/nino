#pragma once

#include "Core/Core.h"
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
			m_DebugName = "WindowClosedEvent";
		}

	public:
		const char* WindowName;
	};

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height)
		{
			m_EventID = EVENT_ID(WindowResizedEvent);
			m_DebugName = "WindowResizedEvent";
		}

		const uint32_t& GetWidth() { return m_Width; }
		const uint32_t& GetHeight() { return m_Height; }

	private:
		uint32_t m_Width;
		uint32_t m_Height;
	};


}