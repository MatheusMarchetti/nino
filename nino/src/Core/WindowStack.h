#pragma once

#include "Core/Window.h"

namespace nino
{
	class WindowStack
	{
	public:
		WindowStack() = default;
		~WindowStack() = default;

		operator bool() { return !s_Windows.empty(); }

		void AddWindow(const char* windowName, Window& window);
		void RemoveWindow(const char* windowName);
		Window* GetWindow(const char* windowName);

		static std::unordered_map<const char*, Window*>& GetStack() { return s_Windows; }

		std::unordered_map<const char*, Window*>::iterator begin() { return s_Windows.begin(); }
		std::unordered_map<const char*, Window*>::iterator end() { return s_Windows.end(); }

		std::unordered_map<const char*, Window*>::const_iterator begin() const { return s_Windows.cbegin(); }
		std::unordered_map<const char*, Window*>::const_iterator end() const { return s_Windows.cend(); }

	private:
		static std::unordered_map<const char*, Window*> s_Windows;
	};
}