#pragma once

#define BIND_EVENT(fn) std::bind(&fn, this, std::placeholders::_1)

#define EVENT_ID(event) typeid(event).hash_code()

namespace nino
{
	class Event
	{
		friend class EventManager;

	public:
		operator bool() { return !Handled; }

	protected:
		Event() = default;
		virtual ~Event() = default;
		const size_t& GetEventID() { return m_EventID; }

	protected:
		size_t m_EventID;
		std::string m_DebugName = "Unknown event";

	private:
		bool Handled = false;
	};
}