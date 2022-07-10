#pragma once

#include "Core/Input.h"
#include "Events/Event.h"

namespace nino
{
	class MouseButtonEvent : public Event
	{
	public:
		const MouseCode& GetMouseButton() { return m_MouseCode; }
		const int& GetPositionX() { return m_X; }
		const int& GetPositionY() { return m_Y; }

	protected:
		MouseButtonEvent(const MouseCode& mousecode, int x, int y)
			: m_MouseCode(mousecode), m_X(x), m_Y(y)
		{}

	private:
		MouseCode m_MouseCode;
		int m_X;
		int m_Y;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode& mousecode, int x, int y)
			: MouseButtonEvent(mousecode, x, y)
		{
			m_EventID = EVENT_ID(MouseButtonPressedEvent);
			m_DebugName = "MouseButtonPressedEvent";
		}
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode& mousecode, int x, int y)
			: MouseButtonEvent(mousecode, x, y)
		{
			m_EventID = EVENT_ID(MouseButtonReleasedEvent);
			m_DebugName = "MouseButtonReleasedEvent";
		}
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(int x, int y, int offsetX, int offsetY)
			: m_X(x), m_Y(y), m_OffsetX(offsetX), m_OffsetY(offsetY)
		{
			m_EventID = EVENT_ID(MouseScrolledEvent);
			m_DebugName = "MouseScrolledEvent";
		}

		const int& GetOffsetX() { return m_OffsetX; }
		const int& GetOffsetY() { return m_OffsetY; }
		const int& GetPositionX() { return m_X; }
		const int& GetPositionY() { return m_Y; }

	private:
		int m_OffsetX;
		int m_OffsetY;
		int m_X;
		int m_Y;
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int x, int y)
			: m_X(x), m_Y(y)
		{
			m_EventID = EVENT_ID(MouseMovedEvent);
			m_DebugName = "MouseMovedEvent";
		}

		const int& GetPositionX() { return m_X; }
		const int& GetPositionY() { return m_Y; }

	private:
		int m_X;
		int m_Y;
	};
}