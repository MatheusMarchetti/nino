#pragma once

namespace nino
{
	class Window
	{
	public:		
		void Init(const char* className, const uint32_t& width, const uint32_t& height);
		void Show();

		HWND& GetWindow() { return m_Window; }
		static const uint32_t GetWidth() { return s_Width; }
		static const uint32_t GetHeight() { return s_Height; }

	private:
		HWND m_Window;
		static uint32_t s_Width;
		static uint32_t s_Height;
	};
}