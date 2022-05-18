#pragma once

#include "Core.h"

namespace nino
{
	class CORE_API Window
	{
	public:
		Window(const wchar_t* className, const uint32_t& width, const uint32_t& height);
		~Window() {}
		
		void Show();

		HWND& GetWindow() { return m_Window; }
		const uint32_t GetWidth() { return m_Width; }
		const uint32_t GetHeight() { return m_Height; }

	private:
		HWND m_Window;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}