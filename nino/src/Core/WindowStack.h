#pragma once

#include "Core/Window.h"

namespace nino
{
	class WindowStack
	{
	public:
		WindowStack() = default;
		~WindowStack() = default;

		operator bool();

		void AddWindow(const char* windowName, Window& window);
		void RemoveWindow(const char* windowName);
		Window* GetWindow(const char* windowName);

		std::unordered_map<const char*, Window*>& GetStack() { return m_Windows; }

		std::unordered_map<const char*, Window*>::iterator begin() { return m_Windows.begin(); }
		std::unordered_map<const char*, Window*>::iterator end() { return m_Windows.end(); }

		std::unordered_map<const char*, Window*>::const_iterator begin() const { return m_Windows.cbegin(); }
		std::unordered_map<const char*, Window*>::const_iterator end() const { return m_Windows.cend(); }

	private:
		std::unordered_map<const char*, Window*> m_Windows;
	};
}