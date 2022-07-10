#pragma once

namespace nino
{
	struct WindowDescriptor
	{
		const char* WindowName;
		uint32_t Width;
		uint32_t Height;
		bool Maximized = false;
	};

	class Window
	{
	public:		
		Window(const WindowDescriptor& descriptor);
		void Show();

		HWND& GetHandle() { return m_Window; }
		WindowDescriptor& GetDescriptor() { return m_Descriptor; }
		const uint32_t GetWidth() { return m_Descriptor.Width; }
		const uint32_t GetHeight() { return m_Descriptor.Height; }

	private:
		WindowDescriptor m_Descriptor;
		HWND m_Window;
	};
}