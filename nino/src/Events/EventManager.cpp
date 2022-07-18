#include "corepch.h"
#include "EventManager.h"

#include "Events/ApplicationEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"

#include "imgui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace nino
{
	static constexpr size_t max_events = 10;
	static size_t s_Head;
	static size_t s_Tail;

	EventManager::EventManager()
	{
		s_Head = 0;
		s_Tail = 0;

        s_Instance = this;

		s_EventQueue.resize(max_events);
	}

	void EventManager::QueueEvent(Event* event)
	{
		s_EventQueue[s_Tail] = event;

		s_Tail = (s_Tail + 1) % max_events;
	}

	void EventManager::ProcessEvents()
	{
		while (s_Head != s_Tail)
		{
			DispatchEvent(s_EventQueue[s_Head]);

			s_Head = (s_Head + 1) % max_events;
		}

		MSG msg = {};
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{           
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void EventManager::DispatchEvent(Event* event)
	{
		m_EventCallback(*event);
	}

	LRESULT EventManager::EventHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        {
            return true;
        }

		switch (msg)
		{
		case WM_DESTROY:
		{
			char windowName[100] = "";

			ThrowOnError(GetClassName(hWnd, windowName, 100));

			QueueEvent(new WindowClosedEvent(windowName));

            break;
		}
        case WM_MOVE:
        case WM_SIZE:
        {
            uint16_t width = LOWORD(lParam);
            uint16_t height = HIWORD(lParam);

            s_Instance->DispatchEvent(new WindowResizedEvent(width, height));

            break;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            KeyCode keycode = (KeyCode)wParam;
            uint16_t repeat = LOWORD(lParam);

            QueueEvent(new KeyPressedEvent(keycode, repeat));

            break;
        }
        case WM_CHAR:
        {
            KeyCode keycode = (KeyCode)wParam;

            QueueEvent(new KeyTypedEvent(keycode));

            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            KeyCode keycode = (KeyCode)wParam;

            QueueEvent(new KeyReleasedEvent(keycode));

            break;
        }
        case WM_LBUTTONDOWN:
        {
            MouseCode button = MOUSE_LEFT;
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            QueueEvent(new MouseButtonPressedEvent(button, x, y));

            break;
        }
        case WM_RBUTTONDOWN:
        {
            MouseCode button = MOUSE_RIGHT;
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            QueueEvent(new MouseButtonPressedEvent(button, x, y));

            break;
        }
        case WM_MBUTTONDOWN:
        {
            MouseCode button = MOUSE_WHEEL;
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            QueueEvent(new MouseButtonPressedEvent(button, x, y));

            break;
        }
        case WM_XBUTTONDOWN:
        {
            MouseCode button;

            if (HIWORD(wParam) == XBUTTON1)
                button = MOUSE_EXTRA1;
            else if (HIWORD(wParam) == XBUTTON2)
                button = MOUSE_EXTRA2;

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            QueueEvent(new MouseButtonPressedEvent(button, x, y));
         
            break;
        }
        case WM_LBUTTONUP:
        {
            MouseCode button = MOUSE_LEFT;
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            QueueEvent(new MouseButtonReleasedEvent(button, x, y));

            break;
        }
        case WM_RBUTTONUP:
        {
            MouseCode button = MOUSE_RIGHT;
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            QueueEvent(new MouseButtonReleasedEvent(button, x, y));
          
            break;
        }
        case WM_MBUTTONUP:
        {
            MouseCode button = MOUSE_WHEEL;
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            QueueEvent(new MouseButtonReleasedEvent(button, x, y));
            
            break;
        }
        case WM_XBUTTONUP:
        {
            MouseCode button;

            if (HIWORD(wParam) == XBUTTON1)
                button = MOUSE_EXTRA1;
            else if (HIWORD(wParam) == XBUTTON2)
                button = MOUSE_EXTRA2;

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            QueueEvent(new MouseButtonReleasedEvent(button, x, y));
            
            break;
        }
        case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            QueueEvent(new MouseMovedEvent(x, y));
            
            break;
        }
        case WM_MOUSEWHEEL:
        {
            int offset = GET_WHEEL_DELTA_WPARAM(wParam);
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            int wheelOffsetX = 0; // currently not implemented
            int wheelOffsetY = 0;
            int deltaCarry = 0;
            deltaCarry += offset;

            while (deltaCarry >= WHEEL_DELTA)
            {
                deltaCarry -= WHEEL_DELTA;
                wheelOffsetY++;
            }
            while (deltaCarry <= -WHEEL_DELTA)
            {
                deltaCarry += WHEEL_DELTA;
                wheelOffsetY--;
            }

            QueueEvent(new MouseScrolledEvent(x, y, wheelOffsetX, wheelOffsetY));
            
            break;
        }
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}
}