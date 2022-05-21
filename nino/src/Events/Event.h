#pragma once

#include "Core/Core.h"

namespace nino
{
	class CORE_API Event
	{
	public:
		virtual ~Event() = default;
		virtual std::wstring ToString() const = 0;
		virtual size_t GetEventID() const = 0;

		bool Handled = false;
	};
}
