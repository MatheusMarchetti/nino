#pragma once

#include "Core/Input.h"
#include "Events/Event.h"

namespace nino
{
	class KeyEvent : public Event
	{
	public:
		const KeyCode& GetKeyCode() { return m_KeyCode; }

	protected:
		KeyEvent(const KeyCode& keycode)
			: m_KeyCode(keycode)
		{}

	private:
		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode& keycode, uint16_t repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount)
		{
			m_EventID = EVENT_ID(KeyPressedEvent);
			m_DebugName = "KeyPressedEvent";
		}

	private:
		uint16_t m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode& keycode)
			: KeyEvent(keycode)
		{
			m_EventID = EVENT_ID(KeyReleasedEvent);
			m_DebugName = "KeyReleasedEvent";
		}

	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode& keycode)
			: KeyEvent(keycode)
		{
			m_EventID = EVENT_ID(KeyTypedEvent);
			m_DebugName = "KeyTypedEvent";
		}
	};
}