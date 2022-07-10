#include "corepch.h"
#include "WindowStack.h"

namespace nino
{
	std::unordered_map<const char*, Window*> WindowStack::s_Windows;

	void WindowStack::AddWindow(const char* windowName, Window& window)
	{
		s_Windows[windowName] = &window;
	}

	void WindowStack::RemoveWindow(const char* windowName)
	{
		auto it = s_Windows.find(windowName);

		s_Windows.erase(it);
	}

	Window* WindowStack::GetWindow(const char* windowName)
	{
		return s_Windows[windowName];
	}
}