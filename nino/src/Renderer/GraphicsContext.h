#pragma once

#include "Core.h"
#include "Renderer/GraphicsCore.h"

namespace nino
{
	class CommandManager;
}

namespace nino
{
	class CORE_API GraphicsContext
	{
	public:
		GraphicsContext(const HWND& window, const uint32_t width, const uint32_t height, CommandManager* commandManager);

		Microsoft::WRL::ComPtr<IDXGISwapChain4> GetContext() { return m_SwapChain; }
		bool IsTearingSupported() { return m_TearingSupport; }

		void Release();

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain4> CreateSwapChain(const HWND& window, const uint32_t width, const uint32_t height);
		bool CheckTearingSupport();

	private:
		bool m_TearingSupport;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> m_SwapChain;

		CommandManager* m_CommandManager;
	};
}