#include "corepch.h"
#include "WindowStack.h"

namespace nino
{
	void WindowStack::AddWindow(const char* windowName, Window& window)
	{
		m_Windows[windowName] = &window;
	}

	void WindowStack::RemoveWindow(const char* windowName)
	{
		m_Windows.erase(windowName);
	}

	Window* WindowStack::GetWindow(const char* windowName)
	{
		return m_Windows[windowName];
	}
}