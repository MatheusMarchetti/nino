#pragma once

#include "Core.h"

#include "Events/Event.h"

#include <typeinfo>

namespace nino
{
	class CORE_API WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() : m_EventID(typeid(WindowClosedEvent).hash_code()) {}

		virtual std::wstring ToString() const override
		{
			return L"WindowClosedEvent";
		}

		virtual size_t GetEventID() const override { return m_EventID; }
		virtual bool IsHandled() const override { return Handled; }

	private:
		size_t m_EventID;
	};
}