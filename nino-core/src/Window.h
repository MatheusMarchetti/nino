#pragma once

#include "Core.h"

namespace nino
{
	class CORE_API Window
	{
	public:
		void Create(const wchar_t* className, const uint32_t& width, const uint32_t& height);
		void Show();

		HWND& GetWindow() { return m_Window; }

	private:
		HWND m_Window;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	};
}