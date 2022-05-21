#pragma once

#include "Core/Core.h"

#include "Events/Event.h"

namespace nino
{
	class CORE_API WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() : m_EventID(EVENT_ID(WindowClosedEvent)) {}

		virtual std::wstring ToString() const override
		{
			std::wstringstream wss;
			wss << "WindowClosedEvent";
			return wss.str();
		}

		virtual size_t GetEventID() const override { return m_EventID; }

	private:
		size_t m_EventID;
	};

	class CORE_API WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32_t width, uint32_t height) : m_EventID(EVENT_ID(WindowResizedEvent)), m_Width(width), m_Height(height) {}

		virtual std::wstring ToString() const override
		{
			std::wstringstream wss;
			wss << "WindowResizedEvent: (width: " << m_Width << ", height: " << m_Height << ")";
			return wss.str();
		}

		virtual size_t GetEventID() const override { return m_EventID; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

	private:
		size_t m_EventID;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}