#include "corepch.h"
#include "WindowStack.h"

namespace nino
{
	WindowStack::operator bool()
	{
		bool valid = true;

		for (auto& window : m_Windows)
		{
			if (!window.second)
				valid = false;
		}

		return valid;
	}

	void WindowStack::AddWindow(const char* windowName, Window& window)
	{
		m_Windows[windowName] = &window;
	}

	void WindowStack::RemoveWindow(const char* windowName)
	{
		m_Windows[windowName] = nullptr;
	}

	Window* WindowStack::GetWindow(const char* windowName)
	{
		return m_Windows[windowName];
	}
}