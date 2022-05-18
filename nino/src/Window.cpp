#include "corepch.h"
#include "Window.h"

#include "Events/EventManager.h"

namespace nino
{
	LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

	Window::Window(const wchar_t* className, const uint32_t& width, const uint32_t& height)
		: m_Width(width), m_Height(height)
	{
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

		HINSTANCE hInstance = GetModuleHandle(NULL);
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance = hInstance;
		wc.lpszClassName = className;
		wc.lpfnWndProc = WindowProc;

		ThrowOnError(RegisterClassEx(&wc));

		RECT rect = { 0, 0, (LONG)width, (LONG)height };
		ThrowOnError(AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE));

		m_Window = ThrowOnError(CreateWindow(className, className, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr));

		NINO_CORE_INFO("Window subsystem initialized!");
	}

	void Window::Show()
	{
		ShowWindow(m_Window, SW_SHOW);
	}

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		EventManager* const pWnd = reinterpret_cast<EventManager*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		return pWnd->EventHandler(hWnd, msg, wParam, lParam);
	}
}
