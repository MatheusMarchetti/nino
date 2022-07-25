#pragma once

#include "Events/EventManager.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"

#include "Input/ControllerCodes.h"
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"

namespace nino
{
	class Input
	{
	public:
		static bool IsKeyDown(KeyCode key)
		{
			Event* event = EventManager::SearchForEvent(EVENT_ID(KeyPressedEvent));

			if (const auto e = dynamic_cast<KeyPressedEvent*>(event))
			{
				bool isKey = e->GetKeyCode() == key;
				delete event;
				event = nullptr;

				return isKey;
			}
			else
				return false;
		}

	private:

	};
}