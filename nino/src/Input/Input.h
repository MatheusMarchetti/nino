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

		static bool IsKeyUp(KeyCode key)
		{
			Event* event = EventManager::SearchForEvent(EVENT_ID(KeyReleasedEvent));

			if (const auto e = dynamic_cast<KeyReleasedEvent*>(event))
			{
				bool isKey = e->GetKeyCode() == key;
				delete event;
				event = nullptr;

				return isKey;
			}
			else
				return false;
		}

		static bool IsMouseDown(MouseCode button, int* x = nullptr, int* y = nullptr)
		{
			Event* event = EventManager::SearchForEvent(EVENT_ID(MouseButtonPressedEvent));

			if (const auto e = dynamic_cast<MouseButtonPressedEvent*>(event))
			{
				bool isButton = e->GetMouseButton() == button;
				if(x) *x = e->GetPositionX();
				if(y) *y = e->GetPositionY();
				delete event;
				event = nullptr;

				return isButton;
			}
			else
				return false;
		}

		static bool IsMouseUp(MouseCode button, int* x = nullptr, int* y = nullptr)
		{
			Event* event = EventManager::SearchForEvent(EVENT_ID(MouseButtonReleasedEvent));

			if (const auto e = dynamic_cast<MouseButtonReleasedEvent*>(event))
			{
				bool isButton = e->GetMouseButton() == button;
				if (x) *x = e->GetPositionX();
				if (y) *y = e->GetPositionY();
				delete event;
				event = nullptr;

				return isButton;
			}
			else
				return false;
		}

		static bool IsMouseScrolled(int* offset, int* x = nullptr, int* y = nullptr)
		{
			Event* event = EventManager::SearchForEvent(EVENT_ID(MouseScrolledEvent));

			if (const auto e = dynamic_cast<MouseScrolledEvent*>(event))
			{
				if(offset) *offset = e->GetOffsetY();
				if (x) *x = e->GetPositionX();
				if (y) *y = e->GetPositionY();
				delete event;
				event = nullptr;

				return true;
			}
			else
				return false;
		}
	};
}