#pragma once

namespace nino
{
	class Event
	{
	friend class EventManager;

	public:
		operator bool() { return !Handled; }

	protected:
		Event() = default;
		size_t GetEventID() { return m_EventID; }

	protected:
		size_t m_EventID;
		std::string m_DebugName = "Unknown event";

	private:
		bool Handled = false;
	};
}