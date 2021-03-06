#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Framebuffer.h"

namespace nino
{
	class Window;
}

namespace nino
{
	class GraphicsAPI
	{
	public:
		~GraphicsAPI();

		static void CreateDeviceAndContext();
		static void CreateSwapChain(Window* window);

		static Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return s_Device; }
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return s_DeviceContext; }
		static const Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain(const Window* window);

		static void SetTextureQuality(float quality) { s_TextureQuality = quality; }

		static void BindFramebuffers(std::initializer_list<Ref<Framebuffer>> framebuffers);
		static void Present(bool vSync);

	private:
		static Microsoft::WRL::ComPtr<IDXGIAdapter> GetAdapter();

	private:
		inline static float s_TextureQuality;
		inline static BOOL s_TearingSupport = TRUE;
		inline static Microsoft::WRL::ComPtr<ID3D11Device> s_Device;
		inline static Microsoft::WRL::ComPtr<ID3D11DeviceContext> s_DeviceContext;
		inline static std::unordered_map<HWND, Microsoft::WRL::ComPtr<IDXGISwapChain4>> s_RenderTargets;
		inline static std::unordered_map<std::string, Framebuffer*> s_Framebuffers;
	};
}