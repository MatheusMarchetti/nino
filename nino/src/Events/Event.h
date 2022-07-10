#pragma once

namespace nino
{
	class Event
	{
	public:
		Event() = default;

		operator bool() { return !Handled; }

		size_t GetEventID() { return m_EventID; }

	public:
		bool Handled = false;

	protected:
		size_t m_EventID;

	};
}