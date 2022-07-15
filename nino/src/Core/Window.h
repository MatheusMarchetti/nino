#pragma once

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	struct WindowDescriptor
	{
		std::string WindowName;
		uint32_t Width;
		uint32_t Height;
		bool Maximized = false;
	};

	class Window
	{
	public:		
		Window(const WindowDescriptor& descriptor);
		~Window() = default;
		void Show();

		const HWND& GetHandle() const { return m_Window; }

		const Microsoft::WRL::ComPtr<IDXGISwapChain4>& GetSwapChain() const { return GraphicsAPI::GetSwapChain(this); }

		WindowDescriptor& GetDescriptor() { return m_Descriptor; }
		const uint32_t GetWidth() { return m_Descriptor.Width; }
		const uint32_t GetHeight() { return m_Descriptor.Height; }

	private:
		WindowDescriptor m_Descriptor;
		HWND m_Window;
	};
}