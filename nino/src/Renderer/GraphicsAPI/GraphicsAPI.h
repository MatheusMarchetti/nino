#pragma once

#include "Core/Window.h"
#include "Renderer/GraphicsAPI/GraphicsCore.h"

namespace nino
{
	class GraphicsAPI
	{
	public:
		GraphicsAPI() { ThrowOnError(CoInitialize(NULL)); }
		virtual ~GraphicsAPI() { CoUninitialize(); }

		static const Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain(const Window* window) { return s_RenderTargets[window->GetHandle()]; }

		void SetTextureQuality(float quality) { s_TextureQuality = quality; }

		virtual void CreateSwapChain(Window* window) = 0;
		virtual void Present(bool vSync) = 0;

	protected:
		inline static float s_TextureQuality;
		inline static constexpr uint32_t s_BufferCount = 3;
		inline static BOOL s_TearingSupport = TRUE;
		inline static std::unordered_map<HWND, Microsoft::WRL::ComPtr<IDXGISwapChain4>> s_RenderTargets;
	};
}