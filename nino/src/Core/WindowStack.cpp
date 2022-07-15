#include "corepch.h"
#include "WindowStack.h"

namespace nino
{
	WindowStack::operator bool()
	{
		bool valid = false;

		for (auto& window : m_Windows)
		{
			if (window.second)
				valid = true;
		}

		return valid;
	}

	void WindowStack::AddWindow(const std::string& windowName, Window* window)
	{
		m_Windows[windowName] = window;
	}

	void WindowStack::RemoveWindow(const std::string& windowName)
	{
		delete m_Windows[windowName];
		m_Windows[windowName] = nullptr;

		NINO_CORE_WARN("{} window closed", windowName);
	}

	Window* WindowStack::GetWindow(const std::string& windowName)
	{
		return m_Windows[windowName];
	}
}