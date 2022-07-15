#include "corepch.h"
#include "Window.h"

#include "Events/EventManager.h"

namespace nino
{
	Window::Window(const WindowDescriptor& descriptor)
		: m_Descriptor(descriptor)
	{
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

		HINSTANCE hInstance = GetModuleHandle(NULL);
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance = hInstance;
		wc.lpszClassName = m_Descriptor.WindowName.c_str();
		wc.lpfnWndProc = EventManager::EventHandler;

		ThrowOnError(RegisterClassEx(&wc));

		RECT rect = { 0, 0, (LONG)m_Descriptor.Width, (LONG)m_Descriptor.Height };
		ThrowOnError(AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE));

		m_Window = ThrowOnError(CreateWindow(m_Descriptor.WindowName.c_str(), m_Descriptor.WindowName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr));

		NINO_CORE_INFO("{} window initialized!", m_Descriptor.WindowName);
	}

	void Window::Show()
	{
		ShowWindow(m_Window, m_Descriptor.Maximized ? SW_MAXIMIZE : SW_SHOW);
	}
}
