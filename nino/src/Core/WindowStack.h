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

		void AddWindow(const std::string& windowName, Window* window);
		void RemoveWindow(const std::string& windowName);
		Window* GetWindow(const std::string& windowName);

		std::unordered_map<std::string, Window*>& GetStack() { return m_Windows; }

		std::unordered_map<std::string, Window*>::iterator begin() { return m_Windows.begin(); }
		std::unordered_map<std::string, Window*>::iterator end() { return m_Windows.end(); }

		std::unordered_map<std::string, Window*>::const_iterator begin() const { return m_Windows.cbegin(); }
		std::unordered_map<std::string, Window*>::const_iterator end() const { return m_Windows.cend(); }

	private:
		std::unordered_map<std::string, Window*> m_Windows;
	};
}